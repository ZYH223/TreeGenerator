#include "matrix.h"
#include "camera.h"

void printCamera(Vec3f center, Vec3f dir, Vec3f up)
{
	cout << center << endl;
	cout << dir << endl;
	cout << up << endl;
}

void VerifyScreenCoordinate(Vec2f &point)
{
	float x = point.x(), y = point.y();
	if (x < 0.0f) x = 0.0f;
	else if (x > 1.0f) x = 1.0f;
	if (y < 0.0f) y = 0.0f;
	else if (y > 1.0f) y = 1.0f;
	point.Set(x, y);
}

OrthographicCamera::OrthographicCamera(Vec3f c, Vec3f d, Vec3f u, float s)
{
	/*d = Vec3f(1.0f, 0.0f, 1.0f);
	u = Vec3f(1.0f, 1.0f, 1.0f);*/
	tmin = -FLT_MAX;
	center = c;
	size = s;
	direction = d;
	direction.Normalize();
	up = (u - (u.Dot3(direction))*direction);// upһ��ȷ�����ٸı䣬���λ�úͷ���任ʱֻ�ı�screenUp
	up.Normalize();
	screenUp = up;
	//assert(fabs(direction.Dot3(up)) < EPSILON);
	Vec3f::Cross3(horizontal, direction, up);
	//Vec3f::Cross3(horizontal, direction, up);
}

// ====================================================================
// Create an orthographic camera with the appropriate dimensions that
// crops the screen in the narrowest dimension.
// ====================================================================

void OrthographicCamera::glInit(int w, int h)
{
	//printCamera(center, direction, up);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w > h)
		glOrtho(-size / 2.0, size / 2.0, -size * (float)h / (float)w / 2.0, size*(float)h / (float)w / 2.0, 0.5, 400.0);
	else
		glOrtho(-size * (float)w / (float)h / 2.0, size*(float)w / (float)h / 2.0, -size / 2.0, size / 2.0, 0.5, 400.0);
}

// ====================================================================
// Place an orthographic camera within an OpenGL scene
// ====================================================================

void OrthographicCamera::glPlaceCamera(void)
{
	//printCamera(center, direction, up);
	glm::mat4 view = glm::mat4(1.0f);
	/*view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(10*direction.x(), 10*direction.y(), 10*direction.z()),
		glm::vec3(up.x(), up.y(), up.z()));*/
	view = glm::lookAt(glm::vec3(center.x(), center.y(), center.z()),
		glm::vec3(center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z()),
		glm::vec3(up.x(), up.y(), up.z()));
	glMultMatrixf(glm::value_ptr(view));
	/*gluLookAt(center.x(), center.y(), center.z(),
		center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(),
		up.x(), up.y(), up.z());*/
}

// ====================================================================
// dollyCamera, truckCamera, and RotateCamera
//
// Asumptions:
//  - up is really up (i.e., it hasn't been changed
//    to point to "screen up")
//  - up and direction are normalized
// Special considerations:
//  - If your constructor precomputes any vectors for
//    use in 'generateRay', you will likely to recompute those
//    values at athe end of the these three routines
// ====================================================================

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================

void OrthographicCamera::dollyCamera(float dist)
{
	center += direction * dist * (size/5.0f);

	// ===========================================
	// ASSIGNMENT 3: Fix any other affected values
	// ===========================================

}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================

void OrthographicCamera::truckCamera(float dx, float dy)
{
	Vec3f horizontal;
	Vec3f::Cross3(horizontal, direction, up);
	horizontal.Normalize();

	Vec3f screenUp;
	Vec3f::Cross3(screenUp, horizontal, direction);

	center += horizontal * dx * (size / 5.0) + screenUp * dy * (size / 5.0f);

	// ===========================================
	// ASSIGNMENT 3: Fix any other affected values
	// ===========================================


}

// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================

void OrthographicCamera::rotateCamera(float rx, float ry)
{
	Vec3f horizontal;
	Vec3f::Cross3(horizontal, direction, up);
	horizontal.Normalize();

	// Don't let the model flip upside-down (There is a singularity
	// at the poles when 'up' and 'direction' are aligned)
	float tiltAngle = acos(up.Dot3(direction));
	if (tiltAngle - ry > 3.13f)
		ry = tiltAngle - 3.13f;
	else if (tiltAngle - ry < 0.01f)
		ry = tiltAngle - 0.01f;

	Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
	rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

	rotMat.Transform(center);
	rotMat.TransformDirection(direction);
}

// ====================================================================
// scaleCamera: Scale the view of the camera
// ====================================================================

void OrthographicCamera::scaleCamera(float ratio)
{
	size *= ratio;
}

PerspectiveCamera::PerspectiveCamera(Vec3f c, Vec3f d, Vec3f u, float a)
{
	tmin = 0;
	center = c;
	fov = a;
	zNear = 0.5f;
	zFar = 20.0f;
	direction = d;
	direction.Normalize();
	up = (u - (u.Dot3(direction))*direction);
	up.Normalize();
	//assert(fabs(direction.Dot3(up)) < EPSILON);
	Vec3f::Cross3(horizontal, direction, up);
}

// ====================================================================
// ====================================================================

// ====================================================================
// Create a perspective camera with the appropriate dimensions that
// crops or stretches in the x-dimension as necessary
// ====================================================================

void PerspectiveCamera::glInit(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(fov, (float)w / (float)h, zNear, zFar);
	glMultMatrixf(glm::value_ptr(projection));
	//gluPerspective(angle*180.0 / 3.14159, (float)w / float(h), 0.5, 40.0);
}

// ====================================================================
// Place a perspective camera within an OpenGL scene
// ====================================================================

void PerspectiveCamera::glPlaceCamera(void)
{
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(center.x(), center.y(), center.z()),
		glm::vec3(center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z()),
		glm::vec3(up.x(), up.y(), up.z()));
	glMultMatrixf(glm::value_ptr(view));
	/*gluLookAt(center.x(), center.y(), center.z(),
		center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(),
		up.x(), up.y(), up.z());*/
}

// ====================================================================
// dollyCamera, truckCamera, and RotateCamera
//
// Asumptions:
//  - up is really up (i.e., it hasn't been changed
//    to point to "screen up")
//  - up and direction are normalized
// Special considerations:
//  - If your constructor precomputes any vectors for
//    use in 'generateRay', you will likely to recompute those
//    values at athe end of the these three routines
// ====================================================================

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================

void PerspectiveCamera::dollyCamera(float dist)
{
	center += direction * dist;

	// ===========================================
	// ASSIGNMENT 3: Fix any other affected values
	// ===========================================


}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================

void PerspectiveCamera::truckCamera(float dx, float dy)
{
	Vec3f horizontal;
	Vec3f::Cross3(horizontal, direction, up);
	horizontal.Normalize();

	Vec3f screenUp;
	Vec3f::Cross3(screenUp, horizontal, direction);

	center += horizontal * dx + screenUp * dy;

	// ===========================================
	// ASSIGNMENT 3: Fix any other affected values
	// ===========================================


}

// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================

void PerspectiveCamera::rotateCamera(float rx, float ry)
{
	Vec3f horizontal;
	Vec3f::Cross3(horizontal, direction, up);
	horizontal.Normalize();

	// Don't let the model flip upside-down (There is a singularity
	// at the poles when 'up' and 'direction' are aligned)
	float tiltAngle = acos(up.Dot3(direction));
	if (tiltAngle - ry > 3.13f)
		ry = tiltAngle - 3.13f;
	else if (tiltAngle - ry < 0.01f)
		ry = tiltAngle - 0.01f;

	Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
	rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

	rotMat.Transform(center);
	rotMat.TransformDirection(direction);
	direction.Normalize();

}

// ====================================================================
// scaleCamera: Scale the view of the camera
// ====================================================================

void PerspectiveCamera::scaleCamera(float ratio)
{
	fov *= ratio;
}