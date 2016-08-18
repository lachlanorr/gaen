'''
Message definitions used to generate cpp Reader and Writer classes.

This is a python file, but shouldn't be used directly, it will be
consumed when build_messages.py is run.
'''

# A single, 32 byte task
class InsertTask(FieldHandler):
    owner = u32Field(payload=True,
                     type_name='thread_id',
                     includes=['core/threading.h'])
    task  = TaskField()

class TaskStatus(FieldHandler):
    status = i32Field(payload=True,
                      type_name='TaskStatus',
                      includes=['engine/Task.h'])

# Instantiate and insert a component
class InsertComponent(FieldHandler):
    nameHash = u32Field(payload=True)
    index = u32Field()

# Transform and entity
class Transform(FieldHandler):
    isLocal   = boolField(payload=True)
    transform = mat4x3Field()

# Transform and entity
class TransformUid(FieldHandler):
    uid       = u32Field(payload=True)
    transform = mat4x3Field()

class InsertModelInstance(FieldHandler):
    uid             = u32Field(payload=True)
    pModel          = PointerField(type_name='Model *',
                                   includes=['engine/Model.h'])
    isAssetManaged  = boolField()
    transform  = mat4x3Field()
    
class InsertLightDirectional(FieldHandler):
    uid       = u32Field(payload=True)
    color     = ColorField()
    direction = vec3Field()

class UpdateLightDirectional(FieldHandler):
    uid       = u32Field(payload=True)
    color     = ColorField()
    direction = vec3Field()

class SpriteInstance(FieldHandler):
    pSpriteInstance = PointerField(type_name='SpriteInstance *',
                      includes=['engine/Sprite.h'])

class SpritePlayAnim(FieldHandler):
    uid       = u32Field(payload=True)
    animHash  = u32Field()
    duration  = f32Field()

class SpriteAnim(FieldHandler):
    uid          = u32Field(payload=True)
    animHash     = u32Field()
    animFrameIdx = u32Field()

class SpriteVelocity(FieldHandler):
    uid          = u32Field(payload=True)
    velocity     = vec2Field()

class SpriteBody(FieldHandler):
    uid          = u32Field(payload=True)
    mass         = f32Field()

class MoveCamera(FieldHandler):
    position = vec3Field()
    direction = quatField()

class MouseMove(FieldHandler):
    xDelta = u32Field(payload=True)
    yDelta = u32Field()

class Handle(FieldHandler):
    taskId = u32Field(payload=True)
    nameHash = u32Field()
    pHandle = PointerField(type_name='Handle *',
                           includes=['engine/Handle.h'])

class Asset(FieldHandler):
    taskId = u32Field(payload=True)
    subTaskId = u32Field()
    nameHash = u32Field()
    pAsset = PointerField(type_name='Asset *',
                          includes=['engine/Asset.h'])
