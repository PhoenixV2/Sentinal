#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

#include <vector>

class VertexBuffer{
	private:
		unsigned int _rendererId;
	
	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();
		void Bind() const;
		void Unbind() const;
};

struct VertexBufferElement{
	unsigned int type;
	unsigned int numIndeces;
	unsigned char normalised; 
};

class VertexArray{
	private:
		unsigned int _rendererId;
		unsigned int _stride;

		std::vector<VertexBufferElement> _elements;

	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer &vb);

		void Bind() const;
		void Unbind() const;

		// template<typename T>
		// void Push(unsigned int numIndeces){
		// 	// static_assert(false);
		// }

		// template<>
		void Push(unsigned int numIndeces){
			_elements.push_back({GL_FLOAT, numIndeces, GL_FALSE});
			_stride += numIndeces;
		}

		// template<>
		// void Push<unsigned int>(unsigned int numIndeces){
		// 	_elements.push_back({GL_UNSIGNED_INT, numIndeces, GL_FALSE});
		// 	_stride += numIndeces;
		// }

		inline const std::vector<VertexBufferElement> GetElements() const& {return _elements;}
		inline unsigned int GetStride() const {return _stride;}
};

class IndexBuffer{
	private:
		unsigned int _rendererId;
		unsigned int _numIndeces;
	
	public:
		IndexBuffer(const unsigned int* data, unsigned int numIndeces);
		~IndexBuffer();
		void Bind() const;
		void Unbind() const;

		inline unsigned int GetNumIndeces() const {return _numIndeces;}
};