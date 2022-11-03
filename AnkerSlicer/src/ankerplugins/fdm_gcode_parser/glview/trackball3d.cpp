#include "trackball3d.h"
#include <glm/gtx/norm.hpp> 
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp> 
const glm::vec3 TrackBall3D::X(1.f, 0.f, 0.f);
const glm::vec3 TrackBall3D::Y(0.f, 1.f, 0.f);
const glm::vec3 TrackBall3D::Z(0.f, 0.f, 1.f);

TrackBall3D::TrackBall3D() :
    mCameraMotionLeftClick(ARC),
    mCameraMotionMiddleClick(ROLL),
    mCameraMotionRightClick(FIRSTPERSON),
    mCameraMotionScroll(ZOOM),
    mHeight(1),
    mIsDragging(false),
    mIsLeftClick(false),
    mIsMiddleClick(false),
    mIsRightClick(false),
    mIsScrolling(false),
    mPanScale(.005f),
    mRollScale(.005f),
    mRollSum(0.f),
    mRotation(1.f, 0, 0, 0),
    mRotationSum(1.f, 0, 0, 0),
    mClickPoint(0.0f, 0.0f),
    mSpeed(1.f),
    mWidth(1),
    mZoomScale(.1f),
    mZoomSum(0.f)
{
}
void TrackBall3D::reset()
{
    mCameraMotionLeftClick = ARC;
    mCameraMotionMiddleClick = ROLL;
    mCameraMotionRightClick = FIRSTPERSON;
    mCameraMotionScroll = ZOOM;
    mHeight = 1;
    mIsDragging = false;
    mIsLeftClick = false;
    mIsMiddleClick = false;
    mIsRightClick = false;
    mIsScrolling = false;
    mPanScale = 0.005f;
    mRollScale = 0.005f;
    mRollSum = 0.0f;
    mRotation = glm::quat(1.f, 0, 0, 0);
    mRotationSum = glm::quat(1.f, 0, 0, 0);
    mSpeed = 1.0f;
    mWidth = 1.0f;
    mZoomScale = 0.1f;
    mZoomSum = 0.0f;
    mClickPoint = glm::vec2(0.0f, 0.0f);
}
TrackBall3D::~TrackBall3D()
{
    
}


char TrackBall3D::clickQuadrant(float x, float y)
{
    float halfw = .5 * mWidth;
    float halfh = .5 * mHeight;

    if (x > halfw) {
        // Opengl image coordinates origin is upperleft.
        if (y < halfh) {
            return 1;
        }
        else {
            return 4;
        }
    }
    else {
        if (y < halfh) {
            return 2;
        }
        else {
            return 3;
        }
    }
}

void TrackBall3D::computeCameraEye(glm::vec3& eye)
{
    glm::vec3 orientation = mRotationSum * Z;

    if (mZoomSum) {
        mTranslateLength += mZoomScale * mZoomSum;
        mZoomSum = 0; // Freeze zooming after applying.
    }

    eye = mTranslateLength * orientation + mCamera->getCenter();
}

void TrackBall3D::computeCameraUp(glm::vec3& up)
{
    up = glm::normalize(mRotationSum * Y);
}

void TrackBall3D::computePan(glm::vec3& pan)
{
    glm::vec2 click = mClickPoint - mPrevClickPoint;
    glm::vec3 look = mCamera->getEye() - mCamera->getCenter();
    float length = glm::length(look);
    glm::vec3 right = glm::normalize(mRotationSum * X);

    pan = (mCamera->getUp() * -click.y + right * click.x) *
        mPanScale * mSpeed * length;
}

void TrackBall3D::computePointOnSphere(
    const glm::vec2& point, glm::vec3& result)
{
    // https://www.opengl.org/wiki/Object_Mouse_Trackball
    float x = (2.f * point.x - mWidth) / mWidth;
    float y = (mHeight - 2.f * point.y) / mHeight;

    float length2 = x * x + y * y;

    if (length2 <= .5) {
        result.z = sqrt(1.0 - length2);
    }
    else {
        result.z = 0.5 / sqrt(length2);
    }

    float norm = 1.0 / sqrt(length2 + result.z * result.z);

    result.x = x * norm;
    result.y = y * norm;
    result.z *= norm;
}

void TrackBall3D::computeRotationBetweenVectors(
    const glm::vec3& u, const glm::vec3& v, glm::quat& result)
{
    float cosTheta = glm::dot(u, v);
    glm::vec3 rotationAxis;
    static const float EPSILON = 1.0e-5f;

    if (cosTheta < -1.0f + EPSILON) {
        // Parallel and opposite directions.
        rotationAxis = glm::cross(glm::vec3(0.f, 0.f, 1.f), u);

        if (glm::length2(rotationAxis) < 0.01) {
            // Still parallel, retry.
            rotationAxis = glm::cross(glm::vec3(1.f, 0.f, 0.f), u);
        }

        rotationAxis = glm::normalize(rotationAxis);
        result = glm::angleAxis(180.0f, rotationAxis);
    }
    else if (cosTheta > 1.0f - EPSILON) {
        // Parallel and same direction.
        result = glm::quat(1, 0, 0, 0);
        return;
    }
    else {
        float theta = acos(cosTheta);
        rotationAxis = glm::cross(u, v);

        rotationAxis = glm::normalize(rotationAxis);
        result = glm::angleAxis(theta * mSpeed, rotationAxis);
    }
}

void TrackBall3D::drag()
{
    if (mPrevClickPoint == mClickPoint) {
        // Not moving during drag state, so skip unnecessary processing.
        return;
    }

    computePointOnSphere(mClickPoint, mStopVector);
    computeRotationBetweenVectors(mStartVector,
        mStopVector,
        mRotation);
    // Reverse so scene moves with cursor and not away due to camera model.
    mRotation = glm::inverse(mRotation);

    drag(mIsLeftClick, mCameraMotionLeftClick);
    drag(mIsMiddleClick, mCameraMotionMiddleClick);
    drag(mIsRightClick, mCameraMotionRightClick);

    // After applying drag, reset relative start state.
    mPrevClickPoint = mClickPoint;
    mStartVector = mStopVector;
}

void TrackBall3D::drag(bool isClicked, CameraMotionType motion)
{
    if (!isClicked) {
        return;
    }

    switch (motion) {
    case ARC:
        dragArc();
        break;
    case FIRSTPERSON:
        dragFirstPerson();
        break;
    case PAN:
        dragPan();
        break;
    case ROLL:
        rollCamera();
        break;
    case ZOOM:
        dragZoom();
        break;
    default: break;
    }
}

void TrackBall3D::dragArc()
{
    mRotationSum *= mRotation; // Accumulate quaternions.

    updateCameraEyeUp(true, true);
}

void TrackBall3D::dragFirstPerson()
{
    glm::vec3 pan;
    computePan(pan);
    mCamera->setCenter(-pan + mCamera->getCenter());
    mCamera->update();
    freezeTransform();
}

void TrackBall3D::dragPan()
{
    glm::vec3 pan;
    computePan(pan);
    mCamera->setCenter(pan + mCamera->getCenter());
    mCamera->setEye(pan + mCamera->getEye());
    mCamera->update();
    freezeTransform();
}

void TrackBall3D::dragZoom()
{
    glm::vec2 dir = mClickPoint - mPrevClickPoint;
    float ax = fabs(dir.x);
    float ay = fabs(dir.y);

    if (ay >= ax) {
        setScrollDirection(dir.y <= 0);
    }
    else {
        setScrollDirection(dir.x <= 0);
    }

    updateCameraEyeUp(true, false);
}

void TrackBall3D::freezeTransform()
{
    if (mCamera) {
        // Opengl is ZYX order.
        // Flip orientation to rotate scene with sticky cursor.
        mRotationSum = glm::inverse(glm::quat(mCamera->getMatrix()));
        mTranslateLength = glm::length(mCamera->getEye() - mCamera->getCenter());
    }
}

Camera3D* TrackBall3D::getCamera()
{
    return mCamera;
}

TrackBall3D::CameraMotionType TrackBall3D::getMotionLeftClick()
{
    return mCameraMotionLeftClick;
}

TrackBall3D::CameraMotionType TrackBall3D::getMotionMiddleClick()
{
    return mCameraMotionMiddleClick;
}

TrackBall3D::CameraMotionType TrackBall3D::getMotionRightClick()
{
    return mCameraMotionRightClick;
}

TrackBall3D::CameraMotionType TrackBall3D::getMotionScroll()
{
    return mCameraMotionScroll;
}

void TrackBall3D::rollCamera()
{
    glm::vec2 delta = mClickPoint - mPrevClickPoint;
    char quad = clickQuadrant(mClickPoint.x, mClickPoint.y);
    switch (quad) {
    case 1:
        delta.y = -delta.y;
        delta.x = -delta.x;
        break;
    case 2:
        delta.x = -delta.x;
        break;
    case 3:
        break;
    case 4:
        delta.y = -delta.y;
    default:
        break;
    }

    glm::vec3 axis = glm::normalize(mCamera->getCenter() - mCamera->getEye());
    float angle = mRollScale * mSpeed * (delta.x + delta.y + mRollSum);
    glm::quat rot = glm::angleAxis(angle, axis);
    mCamera->setUp(rot * mCamera->getUp());
    mCamera->update();
    freezeTransform();
    mRollSum = 0;
}

void TrackBall3D::scroll()
{
    switch (mCameraMotionScroll) {
    case ROLL:
        rollCamera();
        break;
    case ZOOM:
        updateCameraEyeUp(true, false);
        break;
    default: break;
    }
}

void TrackBall3D::setCamera(Camera3D* c)
{
    mCamera = c;
    freezeTransform();
}

void TrackBall3D::setClickPoint(double x, double y)
{
    mPrevClickPoint = mClickPoint;
    mClickPoint.x = x;
    mClickPoint.y = y;
}

void TrackBall3D::setLeftClicked(bool value)
{
    mIsLeftClick = value;
}

void TrackBall3D::setMiddleClicked(bool value)
{
    mIsMiddleClick = value;
}

void TrackBall3D::setMotionLeftClick(CameraMotionType motion)
{
    mCameraMotionLeftClick = motion;
}

void TrackBall3D::setMotionMiddleClick(CameraMotionType motion)
{
    mCameraMotionMiddleClick = motion;
}

void TrackBall3D::setMotionRightClick(CameraMotionType motion)
{
    mCameraMotionRightClick = motion;
}

void TrackBall3D::setMotionScroll(CameraMotionType motion)
{
    mCameraMotionScroll = motion;
}

void TrackBall3D::setRightClicked(bool value)
{
    mIsRightClick = value;
}

void TrackBall3D::setScreenSize(float width, float height)
{
    if (width > 1 && height > 1) {
        mWidth = width;
        mHeight = height;
    }
}

void TrackBall3D::setScrollDirection(bool up)
{
    mIsScrolling = true;
    float inc = 30 * mSpeed * (up ? -1.f : 1.f);
    mZoomSum += inc;
    mRollSum += inc;
}

void TrackBall3D::setSpeed(float s)
{
    mSpeed = s;
}

void TrackBall3D::accelerate(float s)
{
    mSpeed += s;
}

void TrackBall3D::decelerate(float s)
{
    mSpeed -= s;
}

void TrackBall3D::update()
{
    const bool isClick = mIsLeftClick || mIsMiddleClick || mIsRightClick;

    if (!mIsDragging)
    {
        if (isClick)
        {
            mIsDragging = true;
            computePointOnSphere(mClickPoint, mStartVector);
        }
        else if (mIsScrolling) {
            scroll();
            mIsScrolling = false;
        }
    }
    else
    {
        if (isClick)
        {
            drag();
        }
        else
        {
            mIsDragging = false;
        }
    }
}

void TrackBall3D::updateCameraEyeUp(bool eye, bool up)
{
    if (eye) {
        glm::vec3 eye;
        computeCameraEye(eye);
        mCamera->setEye(eye);
    }
    if (up) {
        glm::vec3 up;
        computeCameraUp(up);
        mCamera->setUp(up);
    }
    mCamera->update();
}
