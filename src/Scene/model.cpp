#include "model.hh"

Model::Model()
{
    initializeOpenGLFunctions();
    tMatrix.setToIdentity();
    rMatrix.setToIdentity();
    _anglRot= 0;
    _translate = QVector3D(0,0,0);
    _scale = QVector3D(1,1,1);
    _modelId = 0;
    _position = QVector3D(0,0,0);
}

Model::~Model()
{
    deleteModel();
}

void Model::setVertices(QVector<QVector3D> posVertices, QVector<QVector3D> colorVertices)
{
    setPositionVertices(posVertices);
    setColorVertices(colorVertices);
}

void Model::createMesh(QOpenGLShaderProgram &program, GLuint &_vertexLocation, GLuint &_colorLocation)
{
    program.bind();
    _VAO.create();
    _VAO.bind();

    _VBO_pos.create();
    _VBO_pos.bind();
    _VBO_pos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _VBO_pos.allocate(_posVertices.constData(), _posVertices.size()*sizeof(_posVertices[0]));

    program.enableAttributeArray(_vertexLocation);
    program.setAttributeBuffer(_vertexLocation, GL_FLOAT, 0, 3);

    _VBO_color.create();
    _VBO_color.bind();
    _VBO_color.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _VBO_color.allocate(_colorVertices.constData(), _colorVertices.size()*sizeof(_colorVertices[0]));

    program.enableAttributeArray(_colorLocation);
    program.setAttributeBuffer(_colorLocation, GL_FLOAT, 0, 3);

    _VAO.release();
    program.release();
}

void Model::draw()
{
    if(!_VAO.isCreated()) return;

    _VAO.bind();

    glDrawArrays(GL_TRIANGLES, 0, _posVertices.size());

    _VAO.release();
}

void Model::deleteModel()
{
    _VAO.destroy();
    _VBO_pos.destroy();
    _VBO_color.destroy();
    _posVertices.clear();
    _colorVertices.clear();
}

void Model::setPosition(QVector3D newPosition)
{
    translate(newPosition - _position);
    _position = newPosition;
}

QMatrix4x4 Model::getScaleMatrix() const
{
    QMatrix4x4 matrixScale;
    matrixScale.scale(this->_scale);
    return matrixScale;
}

QMatrix4x4 Model::get_mMatrix() const
{
    return tMatrix*rMatrix*getScaleMatrix();
}

void Model::translate(QVector3D translate)
{
    tMatrix.translate(translate);
    _position += translate;
}

void Model::setRotate(double angle)
{
    rMatrix.setToIdentity();
    rMatrix.rotate(angle, 0,1,0);
    _anglRot = angle;
}

void Model::rotate(double angle)
{
    _anglRot += angle;
    if(_anglRot >= 360) _anglRot = 0;
    if(_anglRot <= -360) _anglRot = 0;
    rMatrix.rotate(angle, 0,1,0);
}

void Model::changeColor(QOpenGLShaderProgram &program, GLuint &_colorLocation, QVector<QVector3D> colorVertices)
{
    setColorVertices(colorVertices);

    program.bind();
    _VAO.bind();
    _VBO_color.destroy();

    _VBO_color.create();
    _VBO_color.bind();
    _VBO_color.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _VBO_color.allocate(_colorVertices.constData(), _colorVertices.size()*sizeof(_colorVertices[0]));

    program.enableAttributeArray(_colorLocation);
    program.setAttributeBuffer(_colorLocation, GL_FLOAT, 0, 3);

    _VBO_color.release();
    _VAO.release();
    program.release();
}

void Model::changeColor(QOpenGLShaderProgram &program, GLuint &_colorLocation)
{
    program.bind();
    _VAO.bind();
    _VBO_color.destroy();

    _VBO_color.create();
    _VBO_color.bind();
    _VBO_color.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _VBO_color.allocate(_colorVertices.constData(), _colorVertices.size()*sizeof(_colorVertices[0]));

    program.enableAttributeArray(_colorLocation);
    program.setAttributeBuffer(_colorLocation, GL_FLOAT, 0, 3);

    _VBO_color.release();
    _VAO.release();
    program.release();
}
