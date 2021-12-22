#include <iostream>
#include <string>
#include <fstream>
#include "CImg.h"

using namespace std;
using namespace cimg_library;
typedef unsigned char BYTE;

const int N = 10e7, GRIS = 300, N_samples = 33;

const int  w = 650, h = 650;

const int di[4] = {1,0,-1,0}, dj[4] = {0,1,0,-1};
int visit[w][h], edges[w][h], A[w][h];
int points_obj [w][h][N_samples];
int points, triangles;
int triangles_obj[N][3];

CImg<BYTE>* img;
CImg<float> brain;

void dfs(int i, int j) {
    visit[i][j] = 1;
    for(int k = 0; k <= 3; k++) {
        int p = i + di[k];
        int q = j + dj[k];
        if(p < 0 || p >= w || q < 0 || q >= h) {
            edges[i][j] = 1;
            continue;
        }
        if (!A[i][j]) {
            edges[i][j] = 1;
            continue;
        }
        if(visit[p][q]) continue;
        dfs(p, q);
    }
    return;
}

void grayscale() {
    for(int x=0;  x < w; x++) {
        for(int y=0; y < h; y++) {
            A[x][y] = (brain)(x, h-1-y,0) + (brain)(x, h-1-y,1) + (brain)(x, h-1-y,2) >= GRIS;
        }
    }
}

void contorno() {
    for(int x=0;  x < w; x++) {
        for(int y=0; y < h; y++) {

            if (visit[x][y]) continue;
            if (!A[x][y]) continue;

            dfs(x, y);
        }
    }
}

void visualizacion() {
    CImgDisplay dis_img((*img), "My first CImg code");
    for(int x=0;  x < w; x++) {
        for(int y=0; y < h; y++) {
            if (edges[x][y]) {
                (*img)(x,h-1-y,0) = (brain)(x, h-1-y,0);
                (*img)(x,h-1-y,1) = (brain)(x, h-1-y,1);
                (*img)(x,h-1-y,2) = (brain)(x, h-1-y,2);
            }

        }
    }

    dis_img.render((*img));
    dis_img.paint();
    while (!dis_img.is_closed()) {
        dis_img.wait();
    }
}

void new_obj(int i) {
    for(int x=0;  x < w; x++) {
        for(int y=0; y < h; y++) {
            if (edges[x][y]) {
                points_obj[x][y][i] = 1;
                points++;
            }
        }
    }


}

void toPLY(string filePath) {
    ofstream file(filePath);
    if (!file) {
        cout << "Error al abrir el archivo\n";
        exit(EXIT_FAILURE);
    }
    file << "ply\n" <<
            "format ascii 1.0\n" <<
            "comment zipper output\n" <<
            "comment modified by flipply\n" <<
            "element vertex " << points << "\n" <<
            "property float32 x\n" <<
            "property float32 y\n" <<
            "property float32 z\n" <<
            "element face " << triangles << "\n" <<
            "property list uint8 int32 vertex_indices\n" <<
            "end_header\n";

    for(int z = 1; z <= N_samples; z++) {
        for(int x=0;  x < w; x++) {
            for(int y=0; y < h; y++) {
                if(points_obj[x][y][z]) {
                    file << float(x)/float(w) << ' ' << float(y)/float(h) << ' ' << float(z)/float(2*N_samples) << "\n";
                }
            }
        }
    }
}

void toTXT(string filePath) {
    ofstream file(filePath);
    if (!file) {
        cout << "Error al abrir el archivo\n";
        exit(EXIT_FAILURE);
    }
    file << points << "\n";

    for(int x=0;  x < w; x++) {
        for(int y=0; y < h; y++) {
            for(int z = 1; z <= N_samples; z++) {
                if(points_obj[x][y][z]) {
                    file << float(x)/float(w) << ' ' << float(y)/float(h) << ' ' << float(z)/float(2*N_samples) << "\n";
                }
            }
        }
    }
}

int main()
{
    img = new CImg<BYTE>(w, h, 1, 3);
    img->fill(0);
    string filePath = "../brain_bmp/";

    for(int i = 1; i <= N_samples; i++) {

        filePath += to_string(i) + ".bmp";

        brain.load(filePath.c_str());

        grayscale();

        contorno();

        visualizacion();
        //new_obj(i);

        for(int i = 0; i < 5; i++) filePath.pop_back();
        if(i/10) filePath.pop_back();
    }

    //toTXT("points.txt");

    return 0;
}