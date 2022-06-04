#include "renderer.h"

VertexArray::VertexArray():_stride(0){
	glGenVertexArrays(1, &_rendererId);
}

VertexArray::~VertexArray(){
	glDeleteVertexArrays(1, &_rendererId);
}

void VertexArray::AddBuffer(const VertexBuffer &vb){
	vb.Bind();
	Bind();

	unsigned int offset = 0;
	for(unsigned int i=0; i < _elements.size(); i++){
		const auto& element = _elements[i];
		glVertexAttribPointer(i, element.numIndeces, element.type, element.normalised, _stride*sizeof(float), (void*)(offset*sizeof(float)));
		glEnableVertexAttribArray(i);
		offset += element.numIndeces;
	}
}

void VertexArray::Bind() const{
	glBindVertexArray(_rendererId);
}

void VertexArray::Unbind() const{
	glBindVertexArray(0);
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size){
	glGenBuffers(1, &_rendererId);
	glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer(){
	glDeleteBuffers(1, &_rendererId);
}

void VertexBuffer::Bind() const{
	glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
}

void VertexBuffer::Unbind() const{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int numIndeces):_numIndeces(numIndeces){
	glGenBuffers(1, &_rendererId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndeces*sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer(){
	glDeleteBuffers(1, &_rendererId);
}

void IndexBuffer::Bind() const{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId);
}

void IndexBuffer::Unbind() const{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}