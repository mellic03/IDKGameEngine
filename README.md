
## Native Model Format

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

