# `framebuffer` example

The example creates an 8 × 8 RGB framebuffer and fills it with a moving HSV
gradient. Rendering is done through a callback: there is no display attached in
this example, so the callback prints the first row of every frame as RGB hex
triplets:

```
I (1234) example: frame 1: ff0000 fe0810 fd1020 ...
```

## What the example shows

- `fb_init()` — allocate an RGB framebuffer with a renderer callback;
- `fb_begin()` / `fb_end()` — take the framebuffer lock and finish the frame;
- `fb_set_pixel_hsv()` — set pixels using HSV colours;
- `fb_render()` — hand the frame to the renderer callback.

## Build and run

```sh
idf.py set-target esp32
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

To send the frames to real hardware, replace `render()` with a function that
pushes `fb->data` to your display or LED strip; `fb->width * fb->height` RGB
pixels are stored row by row, starting at `fb->data`.
