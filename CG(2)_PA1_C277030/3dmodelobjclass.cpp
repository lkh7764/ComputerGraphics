#include "3dmodelobjclass.h"


Model3DObject::Model3DObject(const WCHAR* o_path, const WCHAR* t_path)
{
	path_obj = o_path;
	path_texture = t_path;

	o_texture = new TextureClass();
}
Model3DObject::~Model3DObject() {}


const WCHAR* Model3DObject::GetObjPath() { return path_obj; }
const WCHAR* Model3DObject::GetTexturePath() { return path_texture; }


bool Model3DObject::LoadModel_obj()
{
	ReadFileCounts();
	return true;
}
bool Model3DObject::ReadFileCounts()
{
	ifstream fin;
	char input;

	int vertexCount = 0;
	int textureCount = 0;
	int normalCount = 0;
	int faceCount = 0;

	fin.open(path_obj);

	if (fin.fail() == true) return false;

	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}

		while (input != '\n')
		{
			fin.get(input);
			if (fin.eof())
				break;
		}

		fin.get(input);
	}

	fin.close();

	LoadDataStructures(vertexCount, textureCount, normalCount, faceCount);

	return true;
}
bool Model3DObject::LoadDataStructures(int vertexCount, int textureCount, int normalCount, int faceCount)
{
	XMFLOAT3* vertices, * texcoords, * normals;
	FaceType* faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;

	vertices = new XMFLOAT3[vertexCount];
	if (!vertices) return false;

	texcoords = new XMFLOAT3[textureCount];
	if (!texcoords) return false;

	normals = new XMFLOAT3[normalCount];
	if (!normals) return false;

	faces = new FaceType[faceCount];
	if (!faces) return false;

	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	fin.open(path_obj);
	if (fin.fail() == true) return false;

	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >>
					vertices[vertexIndex].z;

				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}

			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >>
					normals[normalIndex].z;

				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >>
					input2 >> faces[faceIndex].nIndex3 >> faces[faceIndex].vIndex2 >> input2 >>
					faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2 >>
					faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >>
					faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		while (input != '\n')
		{
			fin.get(input);
			if (fin.eof())
				break;
		}

		fin.get(input);
	}

	o_vertexCnt = faceCount * 3;
	o_indexCnt = o_vertexCnt;

	o_model = new ModelType[o_vertexCnt];
	if (!o_model) return false;

	for (int i = 0; i < faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		o_model[i * 3].x = vertices[vIndex].x;
		o_model[i * 3].y = vertices[vIndex].y;
		o_model[i * 3].z = vertices[vIndex].z;

		o_model[i * 3].tu = texcoords[tIndex].x;
		o_model[i * 3].tv = texcoords[tIndex].y;

		o_model[i * 3].nx = normals[nIndex].x;
		o_model[i * 3].ny = normals[nIndex].y;
		o_model[i * 3].nz = normals[nIndex].z;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		o_model[i * 3 + 1].x = vertices[vIndex].x;
		o_model[i * 3 + 1].y = vertices[vIndex].y;
		o_model[i * 3 + 1].z = vertices[vIndex].z;

		o_model[i * 3 + 1].tu = texcoords[tIndex].x;
		o_model[i * 3 + 1].tv = texcoords[tIndex].y;

		o_model[i * 3 + 1].nx = normals[nIndex].x;
		o_model[i * 3 + 1].ny = normals[nIndex].y;
		o_model[i * 3 + 1].nz = normals[nIndex].z;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		o_model[i * 3 + 2].x = vertices[vIndex].x;
		o_model[i * 3 + 2].y = vertices[vIndex].y;
		o_model[i * 3 + 2].z = vertices[vIndex].z;

		o_model[i * 3 + 2].tu = texcoords[tIndex].x;
		o_model[i * 3 + 2].tv = texcoords[tIndex].y;

		o_model[i * 3 + 2].nx = normals[nIndex].x;
		o_model[i * 3 + 2].ny = normals[nIndex].y;
		o_model[i * 3 + 2].nz = normals[nIndex].z;
	}

	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}

	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}

	if (normals)
	{
		delete[] normals;
		normals = 0;
	}

	if (faces)
	{
		delete[] faces;
		faces = 0;
	}

	return true;
}


int Model3DObject::GetVertexCnt()	{ return o_vertexCnt; }
int Model3DObject::GetIndexCnt()	{ return o_indexCnt; }
int Model3DObject::GetTextureCnt()	{ return o_textureCnt; }
int Model3DObject::GetNormalCnt()	{ return o_normalCnt; }
int Model3DObject::GetFaceCnt()		{ return o_faceCnt; }


const ID3D11Buffer* Model3DObject::GetVertexBuffer()	{ return o_vertexBuf;}
const ID3D11Buffer* Model3DObject::GetIndexBuffer()		{ return o_indexBuf; }
const ModelType* Model3DObject::GetModelType()			{ return o_model; }
TextureClass* Model3DObject::GetTexture()				{ return o_texture; }


void Model3DObject::ReleaseTexture()
{
	o_texture->Shutdown();
	delete o_texture;
	o_texture = 0;
}
void Model3DObject::ReleaseIndexBuffer()
{
	o_indexBuf->Release();
	o_indexCnt = 0;
}
void Model3DObject::ReleaseVertexBuffer()
{
	o_vertexBuf->Release();
	o_vertexCnt = 0;
}
void Model3DObject::ReleaseModel()
{
	delete[] o_model;
	o_model = 0;
}


void Model3DObject::SetScale(XMFLOAT3 s) { scale = s; }
void Model3DObject::SetPosition(XMFLOAT3 p) { position = p; }
const XMFLOAT3 Model3DObject::GetScale() { return scale; }
const XMFLOAT3 Model3DObject::GetPosition() { return position; }
