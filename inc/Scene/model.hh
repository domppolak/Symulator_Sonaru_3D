#ifndef MODEL_H
#define MODEL_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>


class Model : public QOpenGLFunctions
{
public:
    Model();
    ~Model();
    void setModelId(uint id);
    void setPositionVertices(QVector<QVector3D> posVertices);
    void setColorVertices(QVector<QVector3D> colorVertices);
    void setVertices(QVector<QVector3D> posVertices, QVector<QVector3D> colorVertices);
    void setPosition(QVector3D newPosition);
    void setPosition(double xCord, double yCord, double zCord);
    void scale(QVector3D scale);
    void scale(double xScale, double yScale, double zScale);
    void translate(QVector3D translate);
    void translate(double xCord, double yCord, double zCord);
    void setRotate(double angle);
    void rotate(double angle);

    QVector3D getScale() const;
    QMatrix4x4 getScaleMatrix() const;
    QMatrix4x4 getRotateMatrix() const;
    QMatrix4x4 getTranslateMatrix() const;
    QMatrix4x4 get_mMatrix() const;
    QVector3D getPostion() const;
    uint getModelId() const;
    QVector<QVector3D> getPositionVertices() const;
    QVector<QVector3D> getColorVertices() const;
    double get_anglRoot() const;

    void deleteModel();
    bool isCreated() const;
    void bind();
    void release();

    void createMesh(QOpenGLShaderProgram &program, GLuint &_vertexLocation, GLuint &_colorLocation);
    void draw();
    void changeColor(QOpenGLShaderProgram &program, GLuint &_colorLocation, QVector<QVector3D> colorVertices);
    void changeColor(QOpenGLShaderProgram &program, GLuint &_colorLocation);
private:
    QOpenGLVertexArrayObject _VAO;
    QOpenGLBuffer _VBO_pos;
    QOpenGLBuffer _VBO_color;
    QVector<QVector3D> _posVertices;
    QVector<QVector3D> _colorVertices;
    QMatrix4x4 tMatrix;
    QMatrix4x4 rMatrix;

    double _anglRot;
    QVector3D _translate;
    QVector3D _scale;
    uint _modelId;

    QVector3D _position;
    QVector3D _color;
};

inline void Model::setModelId(uint id) {this->_modelId=id;}
inline void Model::setPositionVertices(QVector<QVector3D> posVertices){this->_posVertices = posVertices;}
inline void Model::setColorVertices(QVector<QVector3D> colorVertices){this->_colorVertices = colorVertices;}
inline void Model::setPosition(double xCord, double yCord, double zCord){setPosition(QVector3D(xCord, yCord, zCord));}
inline void Model::scale(QVector3D scale){this->_scale = scale;}
inline void Model::scale(double xScale, double yScale, double zScale){scale(QVector3D(xScale, yScale, zScale));}
inline void Model::translate(double xCord, double yCord, double zCord){translate(QVector3D(xCord, yCord, zCord));}

inline QVector3D Model::getScale() const {return this->_scale;}
inline QMatrix4x4 Model::getRotateMatrix() const {return this->rMatrix;}
inline QMatrix4x4 Model::getTranslateMatrix() const {return this->tMatrix;}
inline QVector3D Model::getPostion() const {return this->_position;}
inline uint Model::getModelId() const {return this->_modelId;}
inline QVector<QVector3D> Model::getPositionVertices() const{return this->_posVertices;}
inline QVector<QVector3D> Model::getColorVertices() const{return this->_colorVertices;}
inline double Model::get_anglRoot() const {return this->_anglRot;}

inline bool Model::isCreated() const {return this->_VAO.isCreated();}
inline void Model::bind() {this->_VAO.bind();}
inline void Model::release() {this->_VAO.release();}

#endif // MODEL_H
