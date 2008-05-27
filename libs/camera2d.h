#ifndef CAMERA2D_H
#define CAMERA2D_H

#include <GL/glu.h>

/**
 * Kamera 2d dla open GL
 */
class Camera2d {

	public:

	///wsp. rzeczywiste wskazywane przez kamere (na œrodku ekranu)
    double x, y, zoom;    

    ///rzeczywista wysokoœæ i szerokoœæ obrazu (w pikselach)
    int w, h;       

	Camera2d(double x, double y, double zoom)
    : x(x), y(y), zoom(zoom) {};

	~Camera2d();
	
    ///ustawia obraz openGL zgodnie z kamer¹
    void set() const;

    ///zapamiêtuje nowy rozmiar okna
    void resize(int width, int height);
    
    ///przelicza podan¹ wspó³rzêdn¹ ekranow¹ x na rzeczywist¹ (w rysowanym œwiecie)
    double realX(int x) const {
        return this->x + ((double) x - (double)w/2.0) / zoom;
    }
    
    int screenX(double x) const {
    	return (x - this->x) * zoom + double(w)/2.0;
    }
    
    ///przelicza podan¹ wspó³rzêdn¹ ekranow¹ y na rzeczywist¹ (w rysowanym œwiecie)
    double realY(int y) const {
        return this->y + ((double)h/2.0 - (double) y) / zoom;
    }
    
    int screenY(double y) const {
    	return double(h)/2.0 - (y - this->y) * zoom;
    }
    
    double realLeft() const {
        return realX(0);
    }
    
    double realRight() const {
        return realX(w);
    }
    
    double realTop() const {
        return realY(0);
    }
    
    double realBottom() const {
        return realY(h);
    }
    
    void centerToScreen(int x, int y) {
        this->x = realX(x);
        this->y = realY(y);
        set();
    }
    
    void zoomIn(double z) {
        zoom *= z;
        set();
    }
    
    ///przesówa kamere o podany wektor
    void translate(double x, double y) {
        this->x += x;
        this->y += y;
        set();
    }
    
    ///Ustawia normaln¹ szerokoœæ linii (równ¹ 1)
    void normalLine() const {
        glLineWidth(1.0);
    }
    
    ///Ustawia gruboœæ lini uwzglêdniaj¹c zoom
    void zoomedLine() const {
        glLineWidth(zoom*1.4);
    }

};

#endif // CAMERA2D_H

