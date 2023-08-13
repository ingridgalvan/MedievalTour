////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "terrainclass.h"


TerrainClass::TerrainClass()
{
	m_heightMap = 0;
	m_TerrainModel = 0;
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Textures = 0;
}


TerrainClass::TerrainClass(const TerrainClass& other)
{
}


TerrainClass::~TerrainClass()
{
}


bool TerrainClass::Initialize(ID3D11Device* device, char* heightMapFilename, WCHAR** filenames, int textureCount)
{
	bool result;


	// Load in the height map for the terrain.
	result = LoadHeightMap(heightMapFilename);
	if (!result)
	{
		return false;
	}

	// Reduce the height of the height map.
	ReduceHeightMap();

	// Calculate the normals for the terrain data.
	result = CalculateNormals();
	if (!result)
	{
		return false;
	}

	// Construct a 3D model from the height map and normal data.
	result = BuildTerrainModel();
	if (!result)
	{
		return false;
	}

	// Calculate the normal, tangent, and binormal vectors for the terrain model.
	CalculateTerrainVectors();

	// Initialize the vertex and index buffer for the terrain.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the textures.
	result = LoadTextures(device, filenames, textureCount);
	if (!result)
	{
		return false;
	}

	return true;
}


void TerrainClass::Shutdown()
{
	// Release the textures.
	ReleaseTextures();

	// Release the vertex and index buffer.
	ShutdownBuffers();

	// Release the terrain model.
	ReleaseTerrainModel();

	// Release the height map data.
	ShutdownHeightMap();

	return;
}


void TerrainClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int TerrainClass::GetIndexCount()
{
	return m_indexCount;
}


ID3D11ShaderResourceView** TerrainClass::GetTextures()
{
	ID3D11ShaderResourceView** result = new ID3D11ShaderResourceView*[m_textureCount];

	for (int i = 0; i < m_textureCount; i++)
	{
		result[i] = m_Textures[i]->GetTexture();
	}

	return result;
}

int TerrainClass::GetTextureCount()
{
	return m_textureCount;
}


bool TerrainClass::LoadHeightMap(char* filename)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, i, j, k, index;
	unsigned char* bitmapImage;
	unsigned char height;


	// Open the height map file in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Save the dimensions of the terrain.
	m_terrainWidth = bitmapInfoHeader.biWidth;
	m_terrainHeight = bitmapInfoHeader.biHeight;

	// Calculate the size of the bitmap image data.
	imageSize = m_terrainWidth * m_terrainHeight * 3;

	// Allocate memory for the bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Create the structure to hold the height map data.
	m_heightMap = new HeightMapType[m_terrainWidth * m_terrainHeight];
	if (!m_heightMap)
	{
		return false;
	}

	// Initialize the position in the image data buffer.
	k = 0;

	// Read the image data into the height map.
	for (j = 0; j<m_terrainHeight; j++)
	{
		for (i = 0; i<m_terrainWidth; i++)
		{
			height = bitmapImage[k];

			index = (m_terrainWidth * j) + i;

			m_heightMap[index].x = (float)i;
			m_heightMap[index].y = (float)height;
			m_heightMap[index].z = (float)j;

			k += 3;
		}
	}

	// Release the bitmap image data.
	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}


void TerrainClass::ShutdownHeightMap()
{
	if (m_heightMap)
	{
		delete[] m_heightMap;
		m_heightMap = 0;
	}

	return;
}


void TerrainClass::ReduceHeightMap()
{
	int i, j;


	for (j = 0; j<m_terrainHeight; j++)
	{
		for (i = 0; i<m_terrainWidth; i++)
		{
			m_heightMap[(m_terrainWidth * j) + i].y /= 15.0f;
		}
	}

	return;
}


bool TerrainClass::CalculateNormals()
{
	int i, j, index1, index2, index3, index, count;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	VectorType* normals;


	// Create a temporary array to hold the un-normalized normal vectors.
	normals = new VectorType[(m_terrainHeight - 1) * (m_terrainWidth - 1)];
	if (!normals)
	{
		return false;
	}

	// Go through all the faces in the mesh and calculate their normals.
	for (j = 0; j<(m_terrainHeight - 1); j++)
	{
		for (i = 0; i<(m_terrainWidth - 1); i++)
		{
			index1 = (j * m_terrainWidth) + i;
			index2 = (j * m_terrainWidth) + (i + 1);
			index3 = ((j + 1) * m_terrainWidth) + i;

			// Get three vertices from the face.
			vertex1[0] = m_heightMap[index1].x;
			vertex1[1] = m_heightMap[index1].y;
			vertex1[2] = m_heightMap[index1].z;

			vertex2[0] = m_heightMap[index2].x;
			vertex2[1] = m_heightMap[index2].y;
			vertex2[2] = m_heightMap[index2].z;

			vertex3[0] = m_heightMap[index3].x;
			vertex3[1] = m_heightMap[index3].y;
			vertex3[2] = m_heightMap[index3].z;

			// Calculate the two vectors for this face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (m_terrainWidth - 1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
		}
	}

	// Now go through all the vertices and take an average of each face normal that the vertex touches to get the averaged normal for that vertex.
	for (j = 0; j<m_terrainHeight; j++)
	{
		for (i = 0; i<m_terrainWidth; i++)
		{
			// Initialize the sum.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Initialize the count.
			count = 0;

			// Bottom left face.
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_terrainWidth - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Bottom right face.
			if ((i < (m_terrainWidth - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_terrainHeight - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper left face.
			if (((i - 1) >= 0) && (j < (m_terrainHeight - 1)))
			{
				index = (j * (m_terrainHeight - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper right face.
			if ((i < (m_terrainWidth - 1)) && (j < (m_terrainHeight - 1)))
			{
				index = (j * (m_terrainHeight - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Take the average of the faces touching this vertex.
			sum[0] = (sum[0] / (float)count);
			sum[1] = (sum[1] / (float)count);
			sum[2] = (sum[2] / (float)count);

			// Calculate the length of this normal.
			length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

			// Get an index to the vertex location in the height map array.
			index = (j * m_terrainHeight) + i;

			// Normalize the final shared normal for this vertex and store it in the height map array.
			m_heightMap[index].nx = (sum[0] / length);
			m_heightMap[index].ny = (sum[1] / length);
			m_heightMap[index].nz = (sum[2] / length);
		}
	}

	// Release the temporary normals.
	delete[] normals;
	normals = 0;

	return true;
}


bool TerrainClass::BuildTerrainModel()
{
	int i, j, index, index1, index2, index3, index4;


	// Set the number of vertices in the model.
	m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 6;

	// Create the terrain model array.
	m_TerrainModel = new ModelType[m_vertexCount];
	if (!m_TerrainModel)
	{
		return false;
	}

	// Setup the increment size for the second set of textures (alpha map).
	float incrementSizeWidth = 1.0f / (float)m_terrainWidth;
	float incrementSizeHeight = 1.0f / (float)m_terrainHeight;

	// Initialize the texture increments.
	float tu2Left = 0.0f;
	float tu2Right = incrementSizeWidth;
	float tv2Bottom = 1.0f;
	float tv2Top = 1.0f - incrementSizeHeight;

	// Load the terrain model with the height map terrain data.
	index = 0;

	for (j = 0; j<(m_terrainHeight - 1); j++)
	{
		for (i = 0; i<(m_terrainWidth - 1); i++)
		{
			index1 = (m_terrainWidth * j) + i;          // Bottom left.
			index2 = (m_terrainWidth * j) + (i + 1);      // Bottom right.
			index3 = (m_terrainWidth * (j + 1)) + i;      // Upper left.
			index4 = (m_terrainWidth * (j + 1)) + (i + 1);  // Upper right.

															// Upper left.
			m_TerrainModel[index].x = m_heightMap[index3].x;
			m_TerrainModel[index].y = m_heightMap[index3].y;
			m_TerrainModel[index].z = m_heightMap[index3].z;
			m_TerrainModel[index].nx = m_heightMap[index3].nx;
			m_TerrainModel[index].ny = m_heightMap[index3].ny;
			m_TerrainModel[index].nz = m_heightMap[index3].nz;
			m_TerrainModel[index].tu = 0.0f;
			m_TerrainModel[index].tv = 0.0f;
			m_TerrainModel[index].tu2 = tu2Left;
			m_TerrainModel[index].tv2 = tv2Top;
			index++;

			// Upper right.
			m_TerrainModel[index].x = m_heightMap[index4].x;
			m_TerrainModel[index].y = m_heightMap[index4].y;
			m_TerrainModel[index].z = m_heightMap[index4].z;
			m_TerrainModel[index].nx = m_heightMap[index4].nx;
			m_TerrainModel[index].ny = m_heightMap[index4].ny;
			m_TerrainModel[index].nz = m_heightMap[index4].nz;
			m_TerrainModel[index].tu = 1.0f;
			m_TerrainModel[index].tv = 0.0f;
			m_TerrainModel[index].tu2 = tu2Right;
			m_TerrainModel[index].tv2 = tv2Top;
			index++;

			// Bottom left.
			m_TerrainModel[index].x = m_heightMap[index1].x;
			m_TerrainModel[index].y = m_heightMap[index1].y;
			m_TerrainModel[index].z = m_heightMap[index1].z;
			m_TerrainModel[index].nx = m_heightMap[index1].nx;
			m_TerrainModel[index].ny = m_heightMap[index1].ny;
			m_TerrainModel[index].nz = m_heightMap[index1].nz;
			m_TerrainModel[index].tu = 0.0f;
			m_TerrainModel[index].tv = 1.0f;
			m_TerrainModel[index].tu2 = tu2Left;
			m_TerrainModel[index].tv2 = tv2Bottom;
			index++;

			// Bottom left.
			m_TerrainModel[index].x = m_heightMap[index1].x;
			m_TerrainModel[index].y = m_heightMap[index1].y;
			m_TerrainModel[index].z = m_heightMap[index1].z;
			m_TerrainModel[index].nx = m_heightMap[index1].nx;
			m_TerrainModel[index].ny = m_heightMap[index1].ny;
			m_TerrainModel[index].nz = m_heightMap[index1].nz;
			m_TerrainModel[index].tu = 0.0f;
			m_TerrainModel[index].tv = 1.0f;
			m_TerrainModel[index].tu2 = tu2Left;
			m_TerrainModel[index].tv2 = tv2Bottom;
			index++;

			// Upper right.
			m_TerrainModel[index].x = m_heightMap[index4].x;
			m_TerrainModel[index].y = m_heightMap[index4].y;
			m_TerrainModel[index].z = m_heightMap[index4].z;
			m_TerrainModel[index].nx = m_heightMap[index4].nx;
			m_TerrainModel[index].ny = m_heightMap[index4].ny;
			m_TerrainModel[index].nz = m_heightMap[index4].nz;
			m_TerrainModel[index].tu = 1.0f;
			m_TerrainModel[index].tv = 0.0f;
			m_TerrainModel[index].tu2 = tu2Right;
			m_TerrainModel[index].tv2 = tv2Top;
			index++;

			// Bottom right.
			m_TerrainModel[index].x = m_heightMap[index2].x;
			m_TerrainModel[index].y = m_heightMap[index2].y;
			m_TerrainModel[index].z = m_heightMap[index2].z;
			m_TerrainModel[index].nx = m_heightMap[index2].nx;
			m_TerrainModel[index].ny = m_heightMap[index2].ny;
			m_TerrainModel[index].nz = m_heightMap[index2].nz;
			m_TerrainModel[index].tu = 1.0f;
			m_TerrainModel[index].tv = 1.0f;
			m_TerrainModel[index].tu2 = tu2Right;
			m_TerrainModel[index].tv2 = tv2Bottom;
			index++;
			// Increment the tu texture coords for the alpha map.
			tu2Left += incrementSizeWidth;
			tu2Right += incrementSizeWidth;
		}

		// Reset the tu texture coordinate increments for the alpha map.
		tu2Left = 0.0f;
		tu2Right = incrementSizeWidth;

		// Increment the tv texture coords for the alpha map.
		tv2Top -= incrementSizeHeight;
		tv2Bottom -= incrementSizeHeight;
	}

	return true;
}


void TerrainClass::ReleaseTerrainModel()
{
	if (m_TerrainModel)
	{
		delete[] m_TerrainModel;
		m_TerrainModel = 0;
	}

	return;
}


void TerrainClass::CalculateTerrainVectors()
{
	int faceCount, i, index;
	TempVertexType vertex1, vertex2, vertex3;
	VectorType tangent, binormal;


	// Calculate the number of faces in the terrain model.
	faceCount = m_vertexCount / 3;

	// Initialize the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (i = 0; i<faceCount; i++)
	{
		// Get the three vertices for this face from the terrain model.
		vertex1.x = m_TerrainModel[index].x;
		vertex1.y = m_TerrainModel[index].y;
		vertex1.z = m_TerrainModel[index].z;
		vertex1.tu = m_TerrainModel[index].tu;
		vertex1.tv = m_TerrainModel[index].tv;
		vertex1.nx = m_TerrainModel[index].nx;
		vertex1.ny = m_TerrainModel[index].ny;
		vertex1.nz = m_TerrainModel[index].nz;
		index++;

		vertex2.x = m_TerrainModel[index].x;
		vertex2.y = m_TerrainModel[index].y;
		vertex2.z = m_TerrainModel[index].z;
		vertex2.tu = m_TerrainModel[index].tu;
		vertex2.tv = m_TerrainModel[index].tv;
		vertex2.nx = m_TerrainModel[index].nx;
		vertex2.ny = m_TerrainModel[index].ny;
		vertex2.nz = m_TerrainModel[index].nz;
		index++;

		vertex3.x = m_TerrainModel[index].x;
		vertex3.y = m_TerrainModel[index].y;
		vertex3.z = m_TerrainModel[index].z;
		vertex3.tu = m_TerrainModel[index].tu;
		vertex3.tv = m_TerrainModel[index].tv;
		vertex3.nx = m_TerrainModel[index].nx;
		vertex3.ny = m_TerrainModel[index].ny;
		vertex3.nz = m_TerrainModel[index].nz;
		index++;

		// Calculate the tangent and binormal of that face.
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// Store the tangent and binormal for this face back in the model structure.
		m_TerrainModel[index - 1].tx = tangent.x;
		m_TerrainModel[index - 1].ty = tangent.y;
		m_TerrainModel[index - 1].tz = tangent.z;
		m_TerrainModel[index - 1].bx = binormal.x;
		m_TerrainModel[index - 1].by = binormal.y;
		m_TerrainModel[index - 1].bz = binormal.z;

		m_TerrainModel[index - 2].tx = tangent.x;
		m_TerrainModel[index - 2].ty = tangent.y;
		m_TerrainModel[index - 2].tz = tangent.z;
		m_TerrainModel[index - 2].bx = binormal.x;
		m_TerrainModel[index - 2].by = binormal.y;
		m_TerrainModel[index - 2].bz = binormal.z;

		m_TerrainModel[index - 3].tx = tangent.x;
		m_TerrainModel[index - 3].ty = tangent.y;
		m_TerrainModel[index - 3].tz = tangent.z;
		m_TerrainModel[index - 3].bx = binormal.x;
		m_TerrainModel[index - 3].by = binormal.y;
		m_TerrainModel[index - 3].bz = binormal.z;
	}

	return;
}


void TerrainClass::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3, VectorType& tangent,
	VectorType& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;


	// Calculate the two vectors for this face.
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	return;
}

bool TerrainClass::GetHeightAtPosition(float positionX, float positionZ, float& height)
{
	//posX /= deltaX;
	//posZ /= deltaZ;

	//if (posX < 0.0f || posZ < 0.0f || posX >= widthDivisions - 1 || posZ >= depthDivisions - 1) return 0.0f;

	//int x = (int)posX;
	//int z = (int)posZ;

	//float fX = posX - (float)x;
	//float fZ = posZ - (float)z;

	//int index1 = (widthDivisions *   z) + x;    // Bottom left.
	//int index2 = (widthDivisions *   z) + (x + 1);  // Bottom right.
	//int index3 = (widthDivisions * (z + 1)) + x;    // Upper left.
	//int index4 = (widthDivisions * (z + 1)) + (x + 1);  // Upper right.

	//float BL = vertices[index1].position.y;
	//float BR = vertices[index2].position.y;
	//float TL = vertices[index3].position.y;
	//float TR = vertices[index4].position.y;

	//return +(BL * (1.0 - fX) + BR * fX)
	//	* (1.0 - fZ)
	//	+ (TL * (1.0 - fX) + TR * fX)
	//	* (fZ);
	//bilinear interpolation

	if (positionX < 0.0f || positionZ < 0.0f || positionX >= m_terrainWidth - 1 || positionZ >= m_terrainHeight - 1) return 0.0f;

	int x = (int)positionX;
	int z = (int)positionZ;

	float fX = positionX - (float)x;
	float fZ = positionZ - (float)z;

	int index1 = (m_terrainWidth *   z) + x;    // Bottom left.
	int index2 = (m_terrainWidth *   z) + (x + 1);  // Bottom right.
	int index3 = (m_terrainWidth * (z + 1)) + x;    // Upper left.
	int index4 = (m_terrainWidth * (z + 1)) + (x + 1);  // Upper right.

	float BL = m_heightMap[index1].y;
	float BR = m_heightMap[index2].y;
	float TL = m_heightMap[index3].y;
	float TR = m_heightMap[index4].y;

	height = (BL * (1.0 - fX) + BR * fX) * (1.0 - fZ) + (TL * (1.0 - fX) + TR * fX) * (fZ);
}


bool TerrainClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	int i;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	// Set the index count to the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex and index array with data from the terrain model.
	for (i = 0; i<m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_TerrainModel[i].x, m_TerrainModel[i].y, m_TerrainModel[i].z);
		vertices[i].texture = D3DXVECTOR2(m_TerrainModel[i].tu, m_TerrainModel[i].tv);
		vertices[i].texture2 = D3DXVECTOR2(m_TerrainModel[i].tu2, m_TerrainModel[i].tv2);
		vertices[i].normal = D3DXVECTOR3(m_TerrainModel[i].nx, m_TerrainModel[i].ny, m_TerrainModel[i].nz);
		vertices[i].tangent = D3DXVECTOR3(m_TerrainModel[i].tx, m_TerrainModel[i].ty, m_TerrainModel[i].tz);
		vertices[i].binormal = D3DXVECTOR3(m_TerrainModel[i].bx, m_TerrainModel[i].by, m_TerrainModel[i].bz);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void TerrainClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void TerrainClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool TerrainClass::LoadTextures(ID3D11Device* device, WCHAR** filename, int count)
{
	bool result;

	m_textureCount = count;
	m_Textures = new TextureClass*[m_textureCount];

	for (int i = 0; i < m_textureCount; i++)
	{
		// Create the texture object.
		m_Textures[i] = new TextureClass;
		if (!m_Textures)
		{
			return false;
		}

		// Initialize the texture object.
		result = m_Textures[i]->Initialize(device, filename[i]);
		if (!result)
		{
			return false;
		}
	}
	return true;
}


void TerrainClass::ReleaseTextures()
{
	// Release the texture object.
	for (int i = 0; i < m_textureCount; i++)
	{
		if (m_Textures[i])
		{
			m_Textures[i]->Shutdown();
			delete m_Textures[i];
			m_Textures[i] = 0;
		}
	}

	return;
}