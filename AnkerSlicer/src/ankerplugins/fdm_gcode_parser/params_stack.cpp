#include "params_stack.h"

paramsStack::paramsStack(GcodeViewer *hdls)
{
    this->handls = hdls;
    this->btype = hdls->m_scene3d->getVIew();
    this->eye_b = hdls->m_scene3d->getEye();
    this->up_b = hdls->m_scene3d->getUp();
    this->front_b = hdls->m_scene3d->getFront();
    this->pushd_flag = hdls->m_extrusions.role_visibility_flags;
    hdls->m_isNotOffScreen = false;
    this->layer_z_range_saved = hdls->m_layers_z_range;
}

paramsStack::~paramsStack()
{
    this->handls->m_layers_z_range = layer_z_range_saved;
    this->handls->m_extrusions.role_visibility_flags = pushd_flag;
    this->handls->m_isNotOffScreen = true;
    this->handls->m_scene3d->setEye(eye_b.x(), eye_b.y(), eye_b.z());
    this->handls->m_scene3d->setUp(up_b.x(), up_b.y(), up_b.z());
    this->handls->m_scene3d->setFront(front_b.x(), front_b.y(), front_b.z());
    this->handls->m_scene3d->setVerticalAngle(30.0);
    this->handls->m_scene3d->setView(btype);
};
