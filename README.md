
<!-- ## Native Model Format

idk.db acts as a database for model and texture data.

| Extension | Purpose |
|-----------|---------|
| `.idktex` | Binary texture data in 8bpc rgba format |
| `.idkvts` | Binary vertex data in 32-bit format |
| `.idkmdl` | Pain text header file, pairs texture IDs with .idkvts files |


### .idktex
The first 32 bits stores the number of values to be read. \
A 16x16 image contains 256 rgba values, so the first 32 bits will be 1024 (4x256).
```txt
num_values  r g b a  r g b a  r g b a  r g b a
```


### .idkmdl
```txt
textureID vts0ID vts1ID vts2ID ...
```

### .idkvts
```txt
2   pos nrm tan uv  pos nrm tan uv
2   x y z  x y z  x y z  u v   x y z  x y z  x y z  u v
```
 -->


# IDK Game Engine


##  Dependencies
- [OpenGL Mathematics (glm)](https://github.com/g-truc/glm)
- [OpenGL Extension Wrangler (GLEW)](https://github.com/nigels-com/glew)
- [SDL2](https://github.com/libsdl-org/SDL)
- [SDL_image](https://github.com/libsdl-org/SDL_image)


## Shader Standardization
- Texture uniforms follow the below naming convention:
    - un_texture_0
    - un_texture_1
    - un_texture_2
    - un_texture_n

- Deferred lighting
    ```GLSL
    uniform sampler2D un_texture_0;  // albedo + specular
    uniform sampler2D un_texture_1;  // position
    uniform sampler2D un_texture_2;  // normal
    uniform sampler2D un_texture_3;  // emission
    ```


- Globally accessible UBO data
    ```GLSL
    // UBO_camera.glsl
    // --------------------------------------------------
    layout (std140, binding = 2) uniform UBO_camera_data
    {
        mat4 ubo_view;
        mat4 ubo_projection;
        vec3 ubo_viewpos;
    };
    // --------------------------------------------------

    // UBO_lightsources.glsl
    // --------------------------------------------------
    layout (std140, binding = 3) uniform UBO_pointlights
    {
        int         ubo_num_pointlights;
        PointLight  ubo_pointlights[MAX_POINTLIGHTS];
    };

    layout (std140, binding = 4) uniform UBO_spotlights
    {
        int         ubo_num_spotlights;
        SpotLight   ubo_spotlights[MAX_SPOTLIGHTS];
    };

    layout (std140, binding = 5) uniform UBO_dirlights
    {
        int         ubo_num_dirlights;
        DirLight    ubo_dirlights[MAX_DIRLIGHTS];
    };
    // --------------------------------------------------
    ```


## Model Loading
Before loading any models, all textures to be used must first be loaded with `idk::ModelManager::loadTextures( rootpath )`. Where `rootpath` is a directory containing image files.

Afterwards, either .obj + .mtl files or native .mdl files can be loaded using `idk::ModelManager::loadOBJ()` or `idk::ModelManager::loadMDL()` respectively. \
These two methods return an identifying integer which can then be used to reference the model.

Models can be drawn using `idk::RenderEngine::drawModel( shader_id, model_id, transform )` by passing the model identifier as model_id.


```C++
int main()
{
    idk::Engine engine(1000, 1000);
    idk::RenderEngine &ren = engine.rengine();

    GLuint shader_id = idk::glInterface::compileShaderProgram(
        "assets/shaders/",
        "default.vs",
        "default.fs"
    );

    ren.modelManager().loadTextures("assets/textures/");
    int obj_cube = ren.modelManager().loadOBJ("assets/models/", "cube.obj", "cube.mtl");
    int mdl_cube = ren.modelManager().loadMDL("assets/models/", "cube.mdl");

    idk::Transform t1, t2;
    t1.translate(glm::vec3(-1.0f, 0.0f, 0.0f));
    t2.translate(glm::vec3(+1.0f, 0.0f, 0.0f));

    while (engine.running())
    {
        engine.beginFrame();

        ren.drawModel(shader_id, obj_cube, t1);
        ren.drawModel(shader_id, mdl_cube, t2);

        engine.endFrame();
    }

    return 0;
}
```


## Game Objects
Like models, game objects are represented as integers. Calling `idk::Engine::createGameObject()` will return an integer which can be used to reference an object.

## Component Systems
IDK Engine uses an Entity Component System (ECS) to manage game object properties.



```C++
int main()
{
    idk::Engine engine(1000, 1000);
    const int TRANSFORM = engine.registerCS<Transform_CS>("transform");
    const int PHYSICS   = engine.registerCS<Physics_CS>("physics");
    const int GRABBABLE = engine.registerCS<Grabbable_CS>("grabbable");
    
    int obj1 = engine.createGameObject();
    engine.giveComponents(obj1, TRANSFORM, PHYSICS, GRABBABLE);

    /*
        The three component systems can now perform operations on obj1.
    */
}
```

### Stage A and B
The engine will call `idk::ComponentSystem::stage_A()` and `idk::ComponentSystem::stage_B()` at two points during the frame.
| Stage     | Description |
|-----------|-------------|
| stage_A() | Beginning of frame before any internal processes are run. |
| stage_B() | End of frame after engine and rendering logic but before the front and back buffers have been swapped. Mostly useful for UI. |


### ComponentSystem Callbacks
Each callback is run whenever a game object is created, deleted or copied.

- Game object creation
    ```C++
    idk::ComponentSystem::onGameObjectCreation( int obj_id, idk::Engine & )
    ```


- Game object deletion
    ```C++
    idk::ComponentSystem::onGameObjectDeletion( int obj_id, idk::Engine & )
    ```


- Game object copy. This is called when one gameobject is created by using another as a prefab: `idk::engine::createGameObject( prefab_id )`
    ```C++
    idk::ComponentSystem::onGameObjectCopy( int obj_id, idk::Engine & )
    ```


- Game object component assignment. Called when the component system's component is given to the game object.
    ```C++
    idk::ComponentSystem::onComponentAssignment( int obj_id, idk::Engine & )
    ```

### Creating a new Component System
Inherit from `idk::ComponentSystem` to create a new component system.
