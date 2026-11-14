#include "sphere.h"

#include <QOpenGLFunctions>
#include </usr/include/GL/glu.h>
#include "stripeonsphere.h"
#include <istream>
#include <vector>
#include <iostream>
#include <cmath>
#include <sstream>

static const StripeOnSphere sos{25};

Sphere::Sphere() {}

Sphere::Sphere(int size) {
    sp.resize(size);
    r.resize(size);
    x.resize(size);
    y.resize(size);
    z.resize(size);
    vx.resize(size);
    vy.resize(size);
    vz.resize(size);
    wx.resize(size);
    wy.resize(size);
    wz.resize(size);
    nx.resize(size);
    ny.resize(size);
    nz.resize(size);
    tx.resize(size);
    ty.resize(size);
    tz.resize(size);
    sx.resize(size);
    sy.resize(size);
    sz.resize(size);
}

void Sphere::ReadFromFile(FILE *ft)
{
    int b = 0,b1 = 0;
    double q0 = 0,q1 = 1,q2 = 0,q3 = 0;
    int sp_loaded = 0;
    double r_loaded = 0;
    double x_loaded = 0;
    double y_loaded = 0;
    double z_loaded = 0;
    double vx_loaded = 0;
    double vy_loaded = 0;
    double vz_loaded = 0;
    double wx_loaded = 0;
    double wy_loaded = 0;
    double wz_loaded = 0;


    if(fscanf(ft,"%d\t%d\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%d\t%d\n",
             &b,&sp_loaded,&r_loaded,
             &x_loaded,&y_loaded,&z_loaded,
             &q0,&q1,&q2,&q3,
             &vx_loaded,&vy_loaded,&vz_loaded,
             &wx_loaded,&wy_loaded,&wz_loaded,
               &b,&b1) != 18) printf("Sphere : fscanf Error");

    sp.push_back(sp_loaded);
    r.push_back(r_loaded);
    x.push_back(x_loaded);
    y.push_back(y_loaded);
    z.push_back(z_loaded);
    vx.push_back(vx_loaded);
    vy.push_back(vy_loaded);
    vz.push_back(vz_loaded);
    wx.push_back(wx_loaded);
    wy.push_back(wy_loaded);
    wz.push_back(wz_loaded);

    // Quaternion -> axe de rotation et angle
    if(q0 != 0){
        nx.push_back(1 - 2*q2*q2 - 2*q3*q3);
        ny.push_back(2*q1*q2 + 2*q3*q0);
        nz.push_back(2*q1*q3 - 2*q2*q0);

        tx.push_back(2*q1*q2 - 2*q3*q0);
        ty.push_back(1 - 2*q1*q1 - 2*q3*q3);
        tz.push_back(2*q2*q3 + 2*q1*q0);

        sx.push_back(2*q1*q3 + 2*q2*q0);
        sy.push_back(2*q2*q3 - 2*q1*q0);
        sz.push_back(1 - 2*q1*q1 - 2*q2*q2);
    }
    else{
        nx.push_back(1);
        ny.push_back(0);
        nz.push_back(0);
        tx.push_back(0);
        ty.push_back(1);
        tz.push_back(0);
        sx.push_back(0);
        sy.push_back(0);
        sz.push_back(1);
    }
    if(b1 != -9)bd = true;
}



bool Sphere::readFromStream(std::istream& in) {
    int b = 0, b1 = 0;
    double q0 = 0, q1 = 1, q2 = 0, q3 = 0;
    int sp_loaded = 0;
    double r_loaded = 0;
    double x_loaded = 0, y_loaded = 0, z_loaded = 0;
    double vx_loaded = 0, vy_loaded = 0, vz_loaded = 0;
    double wx_loaded = 0, wy_loaded = 0, wz_loaded = 0;

    std::string line;
    if (!std::getline(in, line)) return false;

    std::istringstream iss(line);
    if (!(iss >> b >> sp_loaded >> r_loaded
          >> x_loaded >> y_loaded >> z_loaded
          >> q0 >> q1 >> q2 >> q3
          >> vx_loaded >> vy_loaded >> vz_loaded
          >> wx_loaded >> wy_loaded >> wz_loaded
          >> b >> b1)) {
        std::cerr << "Sphere: parsing error in line: " << line << "\n";
        return false;
    }

    sp.push_back(sp_loaded);
    r.push_back(r_loaded);
    x.push_back(x_loaded); y.push_back(y_loaded); z.push_back(z_loaded);
    vx.push_back(vx_loaded); vy.push_back(vy_loaded); vz.push_back(vz_loaded);
    wx.push_back(wx_loaded); wy.push_back(wy_loaded); wz.push_back(wz_loaded);

    if (q0 != 0) {
        nx.push_back(1 - 2*q2*q2 - 2*q3*q3);
        ny.push_back(2*q1*q2 + 2*q3*q0);
        nz.push_back(2*q1*q3 - 2*q2*q0);

        tx.push_back(2*q1*q2 - 2*q3*q0);
        ty.push_back(1 - 2*q1*q1 - 2*q3*q3);
        tz.push_back(2*q2*q3 + 2*q1*q0);

        sx.push_back(2*q1*q3 + 2*q2*q0);
        sy.push_back(2*q2*q3 - 2*q1*q0);
        sz.push_back(1 - 2*q1*q1 - 2*q2*q2);
    } else {
        nx.push_back(1); ny.push_back(0); nz.push_back(0);
        tx.push_back(0); ty.push_back(1); tz.push_back(0);
        sx.push_back(0); sy.push_back(0); sz.push_back(1);
    }

    if (b1 != -9) bd = true;
    return true;
}


bool Sphere::readFromStream(std::istream& in, int index) {
    int b = 0, b1 = 0;
    double q0 = 0, q1 = 1, q2 = 0, q3 = 0;

    std::string line;
    if (!std::getline(in, line)) return false;

    std::istringstream iss(line);
    if (!(iss >> b >> sp[index] >> r[index]
          >> x[index] >> y[index] >> z[index]
          >> q0 >> q1 >> q2 >> q3
          >> vx[index] >> vy[index] >> vz[index]
          >> wx[index] >> wy[index] >> wz[index]
          >> b >> b1)) {
        std::cerr << "Sphere: parsing error in line: " << line << "\n";
        return false;
    }

    if (q0 != 0) {
        nx[index] = 1 - 2*q2*q2 - 2*q3*q3;
        ny[index] = 2*q1*q2 + 2*q3*q0;
        nz[index] = 2*q1*q3 - 2*q2*q0;

        tx[index] = 2*q1*q2 - 2*q3*q0;
        ty[index] = 1 - 2*q1*q1 - 2*q3*q3;
        tz[index] = 2*q2*q3 + 2*q1*q0;

        sx[index] = 2*q1*q3 + 2*q2*q0;
        sy[index] = 2*q2*q3 - 2*q1*q0;
        sz[index] = 1 - 2*q1*q1 - 2*q2*q2;
    } else {
        nx[index] = 1; ny[index] = 0; nz[index] = 0;
        tx[index] = 0; ty[index] = 1; tz[index] = 0;
        sx[index] = 0; sy[index] = 0; sz[index] = 1;
    }

    if (b1 != -9) bd = true;
    return true;
}

void Sphere::Draw(int n){
    sos.draw(x[n], y[n], z[n], r[n],
             nx[n], ny[n], nz[n],
             tx[n], ty[n], tz[n],
             sx[n], sy[n], sz[n]);
}
