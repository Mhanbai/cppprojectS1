////////////////////////////////////////////////////////////////////////////////
// Filename: textclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "fontclass.h"
#include "fontshaderclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: TextClass
////////////////////////////////////////////////////////////////////////////////
class TextClass
{
private:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, D3DXMATRIX);
	void Shutdown();
	bool RenderDebugText(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);
	bool RenderMenuText(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);
	bool RenderNetworkText(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);
	bool UpdateSentence(SentenceType*, char*, int, int, float, float, float);

	//Sentences for menu screen
	SentenceType* displayPublicIP;
	SentenceType* displayLocalIP;
	SentenceType* acceptInput;

	//Sentence for network status
	SentenceType* networkStatus;
	SentenceType* networkStatus2;
	SentenceType* networkStatus3;

	//Sentences for in game ui text
	SentenceType* cputext;
	SentenceType* fpstext;
	SentenceType* timetext;
	SentenceType* laptime1text;
	SentenceType* laptime2text;

	//Sentences for debug text
	SentenceType* debug1;
	SentenceType* debug2;
	SentenceType* debug3;

private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, D3DXMATRIX, D3DXMATRIX);

private:
	FontClass* m_Font;
	FontShaderClass* m_FontShader;
	int m_screenWidth, m_screenHeight;
	D3DXMATRIX m_baseViewMatrix;
	ID3D11DeviceContext* m_deviceContext;
};

#endif