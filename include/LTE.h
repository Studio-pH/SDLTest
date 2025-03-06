#ifndef LTE_H
#define LTE_H

struct Camera{
	Camera(int _x, int _y, int _w, int _h, int _zoom) { 
        x = _x; y = _y; w = _w; h = _h; zoom = _zoom;
    }
	int x, y, w, h, zoom;
};

#endif
