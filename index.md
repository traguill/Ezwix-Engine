
Custom enigne created for a subject at university. The engine uses OpenGL and SDL.

**Author**: Guillem Travila

[Repository](https://github.com/traguill/Ezwix-Engine)

[License](https://github.com/traguill/Ezwix-Engine/blob/master/LICENSE.txt)

The following demo shows how to create water using shaders with the engine:

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/dHFYKukKWSc/0.jpg)](https://www.youtube.com/watch?v=dHFYKukKWSc)

### Features

* Hierarchy of GameObjects

* Inspector: shows the information of the selected GameObject and its components

* Resource manager: handles all the resources (meshes, images, scenes...), avoids loading duplicated instances of the same resource, checks for updates on the resources and update them in real time.

* Own file format: all resources imported are converted in a own format and stored in the Library folder.

* Layer system: assign a layer to a GameObject. Cameras can cull objects by layers.

* Render targets: cameras can render to a render target to be used as a texture later on.

* Material creator: allows the creation of materials to assign to GameObjects. The vertex and fragment shader have to be assigned as well as the shader variables in order to see the variables in the inspector. The variables of the shader modified through the inspector are updated at real time. If the vertex or fragment shader code is changed the shader is recompiled and updated also in real time.

* Mouse picking

* Culling: an octree is used to accelerate the culling for the static objects

* Basic illumination: ambient light and directional light values and color can be modified.

### Ri-Karts!

The game made with Ezwix Engine. Check it out [here!](https://citmproject3.github.io/Project3/)

[![Trailer](http://img.youtube.com/vi/Niu3g9-Bpkc/0.jpg)](https://www.youtube.com/watch?v=Niu3g9-Bpkc)

