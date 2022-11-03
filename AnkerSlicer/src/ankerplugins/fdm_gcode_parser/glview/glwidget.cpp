#include "glwidget.h"
#include "input.h"
#include <QCoreApplication>


GLWidget::GLWidget(QWidget* parent) : QOpenGLWidget(parent), QOpenGLFunctions() ,
					m_camera(), timer(new QTimer(this)), m_program(NULL),
					m_cube(NULL), m_texture(NULL)
{
	m_camera.translate(0.0f, 0.0f, 5.0f);	
	grabKeyboard();
}

GLWidget::~GLWidget()
{
	clearGL();
}

void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	//glClearDepthf(1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	QString filename = QCoreApplication::applicationDirPath();
	filename += tr("/../src/ankerplugins/fdm_gcode_parser/");

	m_program = new QOpenGLShaderProgram();
	bool flag = m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, filename + tr("resources/shaders/vshader.glsl"));
	flag = m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, filename + tr("resources/shaders/fshader.glsl"));

	flag = m_program->link();
	flag = m_program->bind();

	m_texture = new QOpenGLTexture(QImage(filename + tr("resources/images/w1.png")).mirrored());
	m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
	m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
	m_texture->setWrapMode(QOpenGLTexture::Repeat);	 

	
	glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);
	m_cube = new Cube;
	m_transform.translate(0.0, 0.0, -5.0);
	timer->start(20);
}

void GLWidget::resizeGL(int w, int h)
{
	m_projection.setToIdentity();
	m_projection.perspective(45.0f, w / float(h), 0.1f, 2000.0f);
}

void GLWidget::horizonsliderMove(int value)
{
	qDebug() << tr("HorizonSlider: ") << value;
}

void GLWidget::verticalsliderMove(int value)
{
	qDebug() << tr("VerticalSlider: ") << value;
}

void GLWidget::paintGL()
{
	//if (m_program == NULL)
	//{
	//	return;
	//}
	//glClearColor(0.9f, 0.9f, 0.9f, 0.9f);
	glClear(GL_COLOR_BUFFER_BIT );
	
	m_texture->bind();
	m_program->setUniformValue("mvp_matrix", m_projection * m_camera.toMatrix()  * m_transform);

	m_program->setAttributeValue("texture", 0);
	m_cube->drawCube(m_program);
}

void GLWidget::loadMesh(const QString& fileName)
{

}

void GLWidget::cleanScene()
{
	
}

void GLWidget::clearGL()
{
	makeCurrent();
	if (m_program != NULL)
	{
		delete m_program;
		m_program = NULL;
	}
	
	if (m_texture != NULL)
	{
		delete m_texture;
		m_texture = NULL;
	}
	if (m_cube != NULL)
	{
		delete m_cube;
	}
	if (timer != NULL)
	{
		delete timer;
		timer = NULL;
	}
	doneCurrent();
}

void GLWidget::update()
{
	Input::update();

	static const float transSpeed = 0.2f;
	static const float rotSpeed = 0.3f;
	static const float cameraSpeed = 0.05f;

	if (Input::buttonPressed(Qt::LeftButton))
	{	
		m_camera.rotate(-rotSpeed * Input::mouseDelta().x(), Camera::LocalUp);
		m_camera.rotate(-rotSpeed * Input::mouseDelta().y(), m_camera.right());
	}
	if (Input::buttonPressed(Qt::RightButton))
	{
		m_camera.translate(cameraSpeed * -Input::mouseDelta().x() * m_camera.right());
		m_camera.translate(cameraSpeed * Input::mouseDelta().y() * m_camera.up());
	}

	QVector3D translation;
	if (Input::keyPressed(Qt::Key_W))
	{
		translation += m_camera.forward();
	}
	if (Input::keyPressed(Qt::Key_S))
	{
		translation -= m_camera.forward();
	}
	if (Input::keyPressed(Qt::Key_A))
	{
		translation -= m_camera.right();
	}
	if (Input::keyPressed(Qt::Key_D))
	{
		translation += m_camera.right();
	}
	if (Input::keyPressed(Qt::Key_Q))
	{
		translation -= m_camera.up();
	}
	if (Input::keyPressed(Qt::Key_E))
	{
		translation += m_camera.up();
	}
	m_camera.translate(transSpeed * translation);

	QOpenGLWidget::update(); 
}

void GLWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->isAutoRepeat())
	{
		event->ignore();
	}
	else
	{
		Input::registerKeyPress(event->key());
	}  
}

void GLWidget::keyReleaseEvent(QKeyEvent* event)
{
	if (event->isAutoRepeat())
	{
		event->ignore();
	}
	else
	{
		Input::registerKeyRelease(event->key());
	} 
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
	Input::registerMousePress(event->button());
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	Input::registerMouseRelease(event->button());
}

void GLWidget::wheelEvent(QWheelEvent* event)
{

	static const float transSpeed = 0.2f;

	QVector3D translation;
	if (event->delta() > 0)
	{
		translation += m_camera.forward();
	}
	else
	{
		translation -= m_camera.forward();
	}
	m_camera.translate(transSpeed * translation);

	QOpenGLWidget::wheelEvent(event); 
}

