# Graphic Assets

[한국어](../한국어/그래픽%20애셋)

Otherengine divides the graphic elements into four main parts.

* Mesh: Vertex data and default material
* Material: Shader and its uniform parameters, and default texture
* Shader: Code written with GLSL
* Texture: png, jpg, ...

Each element has the following format:

## Mesh

* Extension: .omesh
* Format: JSON
* Vertex normal must not be zero vector

```jsonc
{
    "material": "path_to_material",
    "vertices": [
        // [vx, vy, vz, nx, ny, nz, u, v],
        [-0.5, -0.5, -0.5, 0, 0, -1, 0, 0],
        // ...
    ],
    "indices": [
        [2,1,0],
        // ...
    ]
}
```

> NOTE: All paths must be relative to Binaries. ex) ../Assets/Plane.omat

## Material

* Extension: .omat
* Format: JSON
* Supported uniform types: bool, int, float, vecn, ivecn, bvecn, matn, matnxn
* Types are automatically deduced.
* All elements of `vec` must be the same type
* All rows of `mat` must be the same length
* All elements of `mat` are considered float.

```jsonc
{
    "shader": "path_to_shader_without_extension",
    "texture": "path_to_texture",
    "uniforms": {
        "variable_name": /*value*/,
        "bool": false,
        "int": 0,
        "float": 0.0,
        "vec2": [0.0, 0.0],
        "vec4": [0.0, 0.0, 0.0, 0.0],
        "ivec2": [0, 0],
        "bvec2": [false, false],
        "mat3": [
            [0, 0, 0],
            [0, 0, 0],
            [0, 0, 0]
        ],
        "mat2x4": [
            [0, 0, 0, 0],
            [0, 0, 0, 0]
        ],
        // ...
    }
}
```

## [Shader](Shader%20Programming.md)

* Extension: .frag, .vert
* Language: GLSL
* One shader must have a fragment shader (.frag) and a vertex shader (.vert) of the same name.
* Exclude extensions from file paths when referencing shaders.

## Texture

* Extension: .png, .jpg, ... (Supports most image formats)
* Channels: Only RGB(3) and RGBA(4) are supported yet.
* Max size: 65535 x 65535
