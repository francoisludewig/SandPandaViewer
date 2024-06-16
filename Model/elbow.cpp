#include "elbow.h"

#include <QOpenGLFunctions>
#include <GL/GLU.h>

#define N1 15
#define N2 15

Elbow::Elbow() {
    radius = 0.1;
}

void Elbow::ReadFromFile(FILE *ft)
{
    Velocity vx_loaded, vy_loaded, vz_loaded, wx_loaded, wy_loaded, wz_loaded;
    double xi_loaded, yi_loaded, zi_loaded;
    double xf_loaded, yf_loaded, zf_loaded;
    double xr_loaded, yr_loaded, zr_loaded;
    double nx_loaded, ny_loaded, nz_loaded;
    double tx_loaded, ty_loaded, tz_loaded;
    double sx_loaded, sy_loaded, sz_loaded;

    fscanf_s(ft,"%lf\t%lf\t%lf\n",&xi_loaded,&yi_loaded,&zi_loaded);
    fscanf_s(ft,"%lf\t%lf\t%lf\n",&xf_loaded,&yf_loaded,&zf_loaded);
    fscanf_s(ft,"%lf\t%lf\t%lf\n",&xr_loaded,&yr_loaded,&zr_loaded);
    fscanf_s(ft,"%lf\t%lf\t%lf\n",&nx_loaded,&ny_loaded,&nz_loaded);
    fscanf_s(ft,"%lf\t%lf\t%lf\n",&tx_loaded,&ty_loaded,&tz_loaded);
    fscanf_s(ft,"%lf\t%lf\t%lf\n",&sx_loaded,&sy_loaded,&sz_loaded);
    vx_loaded.ReadFromFile(ft);
    vy_loaded.ReadFromFile(ft);
    vz_loaded.ReadFromFile(ft);
    wx_loaded.ReadFromFile(ft);
    wy_loaded.ReadFromFile(ft);
    wz_loaded.ReadFromFile(ft);
    fscanf_s(ft,"%lf\t%lf\t%lf\n",&orx,&ory,&orz);
    fscanf_s(ft,"%lf\t%lf\t%lf\n",&Rc,&alpha,&radius);


    xi.push_back(xi_loaded);
    yi.push_back(yi_loaded);
    zi.push_back(zi_loaded);

    xf.push_back(xf_loaded);
    yf.push_back(yf_loaded);
    zf.push_back(zf_loaded);

    xr.push_back(xr_loaded);
    yr.push_back(yr_loaded);
    zr.push_back(zr_loaded);

    nx.push_back(nx_loaded);
    ny.push_back(ny_loaded);
    nz.push_back(nz_loaded);

    tx.push_back(tx_loaded);
    ty.push_back(ty_loaded);
    tz.push_back(tz_loaded);

    sx.push_back(sx_loaded);
    sy.push_back(sy_loaded);
    sz.push_back(sz_loaded);

    vx.push_back(vx_loaded);
    vy.push_back(vy_loaded);
    vz.push_back(vz_loaded);
    wx.push_back(wx_loaded);
    wy.push_back(wy_loaded);
    wz.push_back(wz_loaded);


}

void Elbow::Draw(int n, bool isLineContainer)
{
    double nrx,nry,nrz;
    double tr_x,tr_y,tr_z;
    double r,A;

    double xc[N1],yc[N1],zc[N1];
    double nxc[N1],nyc[N1],nzc[N1];
    double txc[N1],tyc[N1],tzc[N1];
    double sxc,syc,szc;

    // Memorisation des données du coude
    nrx = nx[n];
    nry = ny[n];
    nrz = nz[n];
    tr_x = tx[n];
    tr_y = ty[n];
    tr_z = tz[n];
    r = radius;
    A = alpha;

    for(int j = 0 ; j <= N1 ; j++){
        xc[j] = xr[n] + Rc * (nrx * sin(j*A/N1) + tr_x * cos(j*A/N1));
        yc[j] = yr[n] + Rc * (nry * sin(j*A/N1) + tr_y * cos(j*A/N1));
        zc[j] = zr[n] + Rc * (nrz * sin(j*A/N1) + tr_z * cos(j*A/N1));
        nxc[j] = (nrx * sin(j*A/N1) + tr_x * cos(j*A/N1));
        nyc[j] = (nry * sin(j*A/N1) + tr_y * cos(j*A/N1));
        nzc[j] = (nrz * sin(j*A/N1) + tr_z * cos(j*A/N1));
        sxc = (nrx * cos(j*A/N1) - tr_x * sin(j*A/N1));
        syc = (nry * cos(j*A/N1) - tr_y * sin(j*A/N1));
        szc = (nrz * cos(j*A/N1) - tr_z * sin(j*A/N1));
        txc[j] = nyc[j]*szc - nzc[j]*syc;
        tyc[j] = nzc[j]*sxc - nxc[j]*szc;
        tzc[j] = nxc[j]*syc - nyc[j]*sxc;
    }

    // Dessin du coude
    double a = 2*3.141592/N2;

    glColor3f(0.5, 0.5, 1);

    for(int j = 0 ; j < N1 ; j++){
        for(int k = 0 ; k < N2 ; k++){
            if(isLineContainer)
                glBegin(GL_LINE_LOOP);
            else
                glBegin(GL_POLYGON);
            glNormal3f(-(nyc[j] * cos(a*k) + tyc[j] * sin(a*k)),
                       -(nzc[j] * cos(a*k) + tzc[j] * sin(a*k)),
                       (nxc[j] * cos(a*k) + txc[j] * sin(a*k)));
            glVertex3f(yc[j] + r * nyc[j] * cos(a*k) + r * tyc[j] * sin(a*k),
                       zc[j] + r * nzc[j] * cos(a*k) + r * tzc[j] * sin(a*k),
                       -(xc[j] + r * nxc[j] * cos(a*k) + r * txc[j] * sin(a*k)));

            glNormal3f(-(nyc[j] * cos(a*(k+1)) + tyc[j] * sin(a*(k+1))),
                       -(nzc[j] * cos(a*(k+1)) + tzc[j] * sin(a*(k+1))),
                       (nxc[j] * cos(a*(k+1)) + txc[j] * sin(a*(k+1))));
            glVertex3f(yc[j] + r * nyc[j] * cos(a*(k+1)) + r * tyc[j] * sin(a*(k+1)),
                       zc[j] + r * nzc[j] * cos(a*(k+1)) + r * tzc[j] * sin(a*(k+1)),
                       -(xc[j] + r * nxc[j] * cos(a*(k+1)) + r * txc[j] * sin(a*(k+1))));

            glNormal3f(-(nyc[(j+1)] * cos(a*(k+1)) + tyc[(j+1)] * sin(a*(k+1))),
                       -(nzc[(j+1)] * cos(a*(k+1)) + tzc[(j+1)] * sin(a*(k+1))),
                       (nxc[(j+1)] * cos(a*(k+1)) + txc[(j+1)] * sin(a*(k+1))));
            glVertex3f(yc[(j+1)] + r * nyc[(j+1)] * cos(a*(k+1)) + r * tyc[(j+1)] * sin(a*(k+1)),
                       zc[(j+1)] + r * nzc[(j+1)] * cos(a*(k+1)) + r * tzc[(j+1)] * sin(a*(k+1)),
                       -(xc[(j+1)] + r * nxc[(j+1)] * cos(a*(k+1)) + r * txc[(j+1)] * sin(a*(k+1))));

            glNormal3f(-(nyc[(j+1)] * cos(a*k) + tyc[(j+1)] * sin(a*k)),
                       -(nzc[(j+1)] * cos(a*k) + tzc[(j+1)] * sin(a*k)),
                       (nxc[(j+1)] * cos(a*k) + txc[(j+1)] * sin(a*k)));
            glVertex3f(yc[(j+1)] + r * nyc[(j+1)] * cos(a*k) + r * tyc[(j+1)] * sin(a*k),
                       zc[(j+1)] + r * nzc[(j+1)] * cos(a*k) + r * tzc[(j+1)] * sin(a*k),
                       -(xc[(j+1)] + r * nxc[(j+1)] * cos(a*k) + r * txc[(j+1)] * sin(a*k)));
            glEnd();
        }
    }
    glColor3f(0, 0, 1);
    // Dessin du coude
    for(int j = 0 ; j < N1 ; j++){
        for(int k = 0 ; k < N2 ; k++){
            if(isLineContainer)
                glBegin(GL_LINE_LOOP);
            else
                glBegin(GL_POLYGON);
            glNormal3f((nyc[j] * cos(a*k) + tyc[j] * sin(a*k)),
                       (nzc[j] * cos(a*k) + tzc[j] * sin(a*k)),
                       -(nxc[j] * cos(a*k) + txc[j] * sin(a*k)));
            glVertex3f(yc[j] + (r*1.001) * nyc[j] * cos(a*k) + (r*1.001) * tyc[j] * sin(a*k),
                       zc[j] + (r*1.001) * nzc[j] * cos(a*k) + (r*1.001) * tzc[j] * sin(a*k),
                       -(xc[j] + (r*1.001) * nxc[j] * cos(a*k) + (r*1.001) * txc[j] * sin(a*k)));

            glNormal3f((nyc[j] * cos(a*(k+1)) + tyc[j] * sin(a*(k+1))),
                       (nzc[j] * cos(a*(k+1)) + tzc[j] * sin(a*(k+1))),
                       -(nxc[j] * cos(a*(k+1)) + txc[j] * sin(a*(k+1))));
            glVertex3f(yc[j] + (r*1.001) * nyc[j] * cos(a*(k+1)) + (r*1.001) * tyc[j] * sin(a*(k+1)),
                       zc[j] + (r*1.001) * nzc[j] * cos(a*(k+1)) + (r*1.001) * tzc[j] * sin(a*(k+1)),
                       -(xc[j] + (r*1.001) * nxc[j] * cos(a*(k+1)) + (r*1.001) * txc[j] * sin(a*(k+1))));

            glNormal3f((nyc[(j+1)] * cos(a*(k+1)) + tyc[(j+1)] * sin(a*(k+1))),
                       (nzc[(j+1)] * cos(a*(k+1)) + tzc[(j+1)] * sin(a*(k+1))),
                       -(nxc[(j+1)] * cos(a*(k+1)) + txc[(j+1)] * sin(a*(k+1))));
            glVertex3f(yc[(j+1)] + (r*1.001) * nyc[(j+1)] * cos(a*(k+1)) + (r*1.001) * tyc[(j+1)] * sin(a*(k+1)),
                       zc[(j+1)] + (r*1.001) * nzc[(j+1)] * cos(a*(k+1)) + (r*1.001) * tzc[(j+1)] * sin(a*(k+1)),
                       -(xc[(j+1)] + (r*1.001) * nxc[(j+1)] * cos(a*(k+1)) + (r*1.001) * txc[(j+1)] * sin(a*(k+1))));

            glNormal3f((nyc[(j+1)] * cos(a*k) + tyc[(j+1)] * sin(a*k)),
                       (nzc[(j+1)] * cos(a*k) + tzc[(j+1)] * sin(a*k)),
                       -(nxc[(j+1)] * cos(a*k) + txc[(j+1)] * sin(a*k)));
            glVertex3f(yc[(j+1)] + (r*1.001) * nyc[(j+1)] * cos(a*k) + (r*1.001) * tyc[(j+1)] * sin(a*k),
                       zc[(j+1)] + (r*1.001) * nzc[(j+1)] * cos(a*k) + (r*1.001) * tzc[(j+1)] * sin(a*k),
                       -(xc[(j+1)] + (r*1.001) * nxc[(j+1)] * cos(a*k) + (r*1.001) * txc[(j+1)] * sin(a*k)));
            glEnd();
        }
    }
}
