#include "solid.h"

Solid::Solid() {}

void Solid::ReadFromFile(FILE *ft)
{
    double x_loaded, y_loaded, z_loaded;
    double q0 = 0,q1 = 1,q2 = 0,q3 = 0;
    Velocity vx_loaded, vy_loaded, vz_loaded, wx_loaded, wy_loaded, wz_loaded;

    fscanf_s(ft, "%lf\t%lf\t%lf",&x_loaded,&y_loaded,&z_loaded);
    fscanf_s(ft, "%lf\t%lf\t%lf\t%lf",&q0,&q1,&q2,&q3);
    vx_loaded.ReadFromFile(ft);
    vy_loaded.ReadFromFile(ft);
    vz_loaded.ReadFromFile(ft);
    wx_loaded.ReadFromFile(ft);
    wy_loaded.ReadFromFile(ft);
    wz_loaded.ReadFromFile(ft);
    fscanf_s(ft, "%lf\t%lf\t%lf",&orx,&ory,&orz);

    x.push_back(x_loaded);
    y.push_back(y_loaded);
    z.push_back(z_loaded);

    nx.push_back(1 - 2*q2*q2 - 2*q3*q3);
    ny.push_back(2*q1*q2 + 2*q3*q0);
    nz.push_back(2*q1*q3 - 2*q2*q0);

    tx.push_back(2*q1*q2 - 2*q3*q0);
    ty.push_back(1 - 2*q1*q1 - 2*q3*q3);
    tz.push_back(2*q2*q3 + 2*q1*q0);

    sx.push_back(2*q1*q3 + 2*q2*q0);
    sy.push_back(2*q2*q3 - 2*q1*q0);
    sz.push_back(1 - 2*q1*q1 - 2*q2*q2);

    vx.push_back(vx_loaded);
    vy.push_back(vy_loaded);
    vz.push_back(vz_loaded);
    wx.push_back(wx_loaded);
    wy.push_back(wy_loaded);
    wz.push_back(wz_loaded);
}
