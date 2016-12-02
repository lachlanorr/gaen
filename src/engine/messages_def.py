'''
Message definitions used to generate cpp Reader and Writer classes.

This is a python file, but shouldn't be used directly, it will be
consumed when build_messages.py is run.
'''

# A single, 32 byte task
class InsertTask(FieldHandler):
    owner = i32Field(payload=True,
                     type_name='thread_id',
                     includes=['core/threading.h'])
    task  = TaskField()

class TaskStatus(FieldHandler):
    status = i32Field(payload=True,
                      type_name='TaskStatus',
                      includes=['engine/Task.h'])

# Instantiate and insert a component
class InsertComponent(FieldHandler):
    nameHash = i32Field(payload=True)
    index = i32Field()

# Transform and entity
class Transform(FieldHandler):
    isLocal   = boolField(payload=True)
    transform = mat4x3Field()

# Transform and entity
class TransformUid(FieldHandler):
    uid       = i32Field(payload=True)
    transform = mat4x3Field()

class InsertLightDirectional(FieldHandler):
    uid       = i32Field(payload=True)
    color     = ColorField()
    direction = vec3Field()

class UpdateLightDirectional(FieldHandler):
    uid       = i32Field(payload=True)
    color     = ColorField()
    direction = vec3Field()

class SpriteInstance(FieldHandler):
    pSpriteInstance = PointerField(type_name='SpriteInstance *',
                                   includes=['engine/Sprite.h'])

class SpritePlayAnim(FieldHandler):
    uid         = i32Field(payload=True)
    animHash    = i32Field()
    duration    = f32Field()
    loop        = boolField()
    doneMessage = i32Field()

class SpriteAnim(FieldHandler):
    uid          = i32Field(payload=True)
    animHash     = i32Field()
    animFrameIdx = i32Field()

class SpriteVelocity(FieldHandler):
    uid          = i32Field(payload=True)
    velocity     = vec2Field()

class SpriteBody(FieldHandler):
    uid          = i32Field(payload=True)
    mass         = f32Field()
    group        = i32Field()
    mask03       = ivec4Field()
    mask47       = ivec4Field()

class ModelInstance(FieldHandler):
    pModelInstance = PointerField(type_name='ModelInstance *',
                                  includes=['engine/Model.h'])

class ModelVelocity(FieldHandler):
    uid          = i32Field(payload=True)
    velocity     = vec3Field()

class ModelBody(FieldHandler):
    uid          = i32Field(payload=True)
    mass         = f32Field()
    group        = i32Field()
    mask03       = ivec4Field()
    mask47       = ivec4Field()

class Collision(FieldHandler):
    group        = i32Field(payload=True)
    subject      = i32Field()
    location     = vec3Field()

class PerspectiveCamera(FieldHandler):
    groupHash    = i32Field(payload=True)
    stageHash    = i32Field()
    cameraHash   = i32Field()
    fieldOfView  = f32Field()
    nearFarClip  = vec2Field()
    transform    = mat4x3Field()

class OrthographicCamera(FieldHandler):
    groupHash    = i32Field(payload=True)
    stageHash    = i32Field()
    cameraHash   = i32Field()
    nearFarClip  = vec2Field()
    transform    = mat4x3Field()

class TransformCamera(FieldHandler):
    groupHash    = i32Field(payload=True)
    cameraHash   = i32Field()
    stageHash    = i32Field()
    transform    = mat4x3Field()

class MoveCamera(FieldHandler):
    position  = vec3Field()
    direction = quatField()

class MouseMove(FieldHandler):
    xDelta = i32Field(payload=True)
    yDelta = i32Field()

class Handle(FieldHandler):
    taskId   = i32Field(payload=True)
    nameHash = i32Field()
    pHandle  = PointerField(type_name='Handle *',
                            includes=['engine/Handle.h'])

class Asset(FieldHandler):
    taskId    = i32Field(payload=True)
    subTaskId = i32Field()
    nameHash  = i32Field()
    pAsset    = PointerField(type_name='Asset *',
                             includes=['engine/Asset.h'])
