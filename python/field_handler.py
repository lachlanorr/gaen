#!/usr/bin/env python

#-------------------------------------------------------------------------------
# field_handler.py - Parse field meta defintions
#
# Gaen Concurrency Engine - http://gaen.org
# Copyright (c) 2014-2016 Lachlan Orr
#
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
#   1. The origin of this software must not be misrepresented; you must not
#   claim that you wrote the original software. If you use this software
#   in a product, an acknowledgment in the product documentation would be
#   appreciated but is not required.
#
#   2. Altered source versions must be plainly marked as such, and must not be
#   misrepresented as being the original software.
#
#   3. This notice may not be removed or altered from any source
#   distribution.
#-------------------------------------------------------------------------------

import new

class BaseField(object):
    def __init__(self, *args, **kwargs):
        for k, v in kwargs.iteritems():
            if type(v) == list: # append to list
                if hasattr(self, k):
                    l = getattr(self, k)
                    l += v
            else: # all other values just set as attributes
                setattr(self, k, v)

    def __str__(self):
        return self.name + ' : ' + repr(self.__dict__)
            
    payload = False
    includes = ['engine/MessageWriter.h']


# cell fields
class cellField(BaseField):
    cell_count = 1

class i32Field(cellField):
    raw_type = 'i32'
    union_type = 'i'

class u32Field(cellField):
    raw_type = 'u32'
    union_type = 'u'

class f32Field(cellField):
    raw_type = 'f32'
    union_type = 'f'
    
class boolField(cellField):
    raw_type = 'bool'
    union_type = 'b'
    
class ColorField(cellField):
    raw_type = 'Color'
    union_type = 'color'
    includes = cellField.includes + ['assets/Color.h']

class EnumField(cellField):
    union_type = 'i'
    members = []

# dcell fields
class dcellField(BaseField):
    cell_count = 2

class i64Field(dcellField):
    raw_type = 'i64'
    union_type = 'i'
    
class u64Field(dcellField):
    raw_type = 'u64'
    union_type = 'u'
    
class f64Field(dcellField):
    raw_type = 'f64'
    union_type = 'f'
    
class PointerField(dcellField):
    union_type = 'p'
    type_name = 'void *'

class vec2Field(BaseField):
    cell_count = 2
    type_name = 'glm::vec2'
    union_type = 'vec2'
    includes = BaseField.includes + ['glm/vec2.hpp']
    
class vec3Field(BaseField):
    cell_count = 3
    type_name = 'glm::vec3'
    includes = BaseField.includes + ['glm/vec3.hpp']

# qcell fields
class vec4Field(BaseField):
    cell_count = 4
    union_type = 'vec4'
    type_name = 'glm::vec4'
    includes = BaseField.includes + ['glm/vec4.hpp']

class quatField(BaseField):
    cell_count = 4
    union_type = 'quat'
    type_name = 'glm::quat'
    includes = BaseField.includes + ['glm/gtc/quaternion.hpp']


# multi-block fields
class TaskField(BaseField):
    cell_count = 8
    includes = BaseField.includes + ['engine/Task.h']

class mat4x3Field(BaseField):
    cell_count = 12
    type_name = 'glm::mat4x3'
    includes = BaseField.includes + ['glm/mat4x3.hpp']


def fields_string(self):
    lines = []
    lines.append(msg.object_name + ':')
    
    lines.append('block_count: ' + str(self.block_count))
    lines.append('attrs: ' + str(self.attrs.__dict__))
    
    s = '%s:\n  %s\n Fields:\n  ' % (msg.object_name, repr(msg.__dict__))
    s += '\n  '.join([str(f) for f in msg.fields])
    return s


def upper_first(s):
    return s[0].upper() + s[1:]

def lower_first(s):
    return s[0].lower() + s[1:]

def block_accessor(field):
    if field.cell_count == 1:
        return 'cells[%d]' % field.block_cell_start
    elif field.cell_count == 2:
        return 'dCells[%d]' % (field.block_cell_start / 2,)
    elif field.cell_count == 3:
        return 'tCellPad.tCell'
    elif field.cell_count == 4:
        return 'qCell'
    else:
        raise Exception('Invalid field size for block accessor')




BLOCK_CELL_COUNT = 4 # cells per block, blocks are 16 bytes
class FieldHandlerType(type):
    '''
    Metaclass to build FieldHandler classes.
    '''
    def __new__(cls, name, bases, attrs):

        if name.startswith('None'):
            return None

        class Attributes:
            def __init__(self):
                self.update = True

        # Go over attributes looking for fields
        has_payload = False
        curr_block = 0
        curr_byte = 0
        fields = []
        includes = []
        newattrs = {'object_name': name,
                    'fields': fields,
                    'includes': includes,
                    'payload_field': None,
                    'attrs': Attributes()}

        field_attrs = []
        for attrname, attrvalue in attrs.iteritems():
            if isinstance(attrvalue, BaseField):
                field_attrs.append((attrname, attrvalue))
            elif not attrname.startswith('_'):
                setattr(newattrs['attrs'], attrname, attrvalue)

        # sort the fields in descending order by cell count
        field_attrs = sorted(field_attrs, key=lambda f: f[1].cell_count, reverse=True)

        # LORRTODO - Fix field packing, make it tighter
        # In some cases we're not packing as efficiently as we can.
        # For example, if there are 2 Vec3s and 2 u32s, we could pack these
        # into 2 blocks, but instead we'll bleed over to a third.
        # The Compose compiler does a nice job of this, and we should be
        # using that algorithm here.

        for fieldname, field in field_attrs:
            field_class_name = field.__class__.__name__
            if not field_class_name.endswith('Field'):
                raise Exception('Field class must end in "Field"')
                
            field.name = fieldname

            if not hasattr(field, 'type_name'):
                field.type_name = field_class_name[:-5]

            for inc in field.includes:
                if inc not in includes:
                    includes.append(inc)

            field.block_count = field.cell_count / BLOCK_CELL_COUNT

            if isinstance(field, PointerField):
                n = field.name
                if n.startswith('p'):
                    n = n[1:]
                field.getter_name = lower_first(n)
            else:
                field.getter_name = field.name

            field.setter_name = 'set' + upper_first(field.getter_name)

            # handle payload
            if field.payload:
                if has_payload:
                    raise Exception('Only one payload field permitted')
                if field.cell_count > 4:
                    raise Exception('Payload must be 4 bytes or less')
                has_payload = True
                newattrs['payload_field'] = field
            # handle block sizing
            else:
                remaining = BLOCK_CELL_COUNT - curr_byte
                if field.cell_count > remaining and curr_byte > 0:
                    curr_block += 1
                    curr_byte = 0
                field.block_start = curr_block
                field.block_cell_start = curr_byte
                curr_byte += field.cell_count % BLOCK_CELL_COUNT
                curr_block += field.cell_count / BLOCK_CELL_COUNT

                if (field.cell_count > BLOCK_CELL_COUNT and field.cell_count % BLOCK_CELL_COUNT != 0):
                    raise Exception("We don't currently support multi block fields that aren't a multiple of 16 bytes")

                # We'll need some data members for the class for larger than block size
                # fields.  This is so we can have a place to copy them into if they
                # happen to wrap the message ring.
                if (field.block_count > 1):
                    field.member_var  = 'm'  + upper_first(field.name)
                    field.member_pvar = 'mp' + upper_first(field.name)

                if field.block_count <= 1:
                    field.block_accessor = block_accessor(field)

            fields.append(field)

                
        newattrs['block_count'] = curr_block + (0 if curr_byte == 0 else 1)
        return super(FieldHandlerType, cls).__new__(cls, name, bases, newattrs)

    def __init__(self, name, bases, attrs):
        super(FieldHandlerType, self).__init__(name, bases, attrs)

        setattr(self.__class__, '__str__', fields_string)
        
        if len(self.fields) > 0:
            FIELD_HANDLERS.append(self)


class FieldHandler(object):
    __metaclass__ = FieldHandlerType


class Component(object):
    __metaclass__ = FieldHandlerType


