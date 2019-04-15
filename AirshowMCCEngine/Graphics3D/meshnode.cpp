#include "meshnode.h"
#include "ResourceCache/resourcecache.h"
#include "AppCode/mainapp.h"

#include "material.h"
#include "texture.h"


std::vector<float> tangents;
std::vector<float> bitangents;

void computeTanBitangents(const tinyobj::mesh_t & mesh, vector<glm::vec3>& tangents, vector<glm::vec3>& bitangents)
{
    //2028 iterations and size of postions and size of texcoords
    for(int i=0; i < int(mesh.positions.size()/3); i+=3)
    {
        glm::vec3 v0(mesh.positions[3*i], mesh.positions[3*i+1], mesh.positions[3*i+2]);
        glm::vec3 v1(mesh.positions[3*(i+1)], mesh.positions[3*(i+1)+1], mesh.positions[3*(i+1)+2]);
        glm::vec3 v2(mesh.positions[3*(i+2)], mesh.positions[3*(i+2)+1], mesh.positions[3*(i+2)+2]);
        glm::vec2 u0(mesh.texcoords[2*i], mesh.texcoords[2*i+1]);
        glm::vec2 u1(mesh.texcoords[2*(i+1)], mesh.texcoords[2*(i+1)+1]);
        glm::vec2 u2(mesh.texcoords[2*(i+2)], mesh.texcoords[2*(i+2)+1]);

        glm::vec3 deltaPos1 = v1-v0;
        glm::vec3 deltaPos2 = v2-v0;
        glm::vec2 deltaUV1 = u1-u0;
        glm::vec2 deltaUV2 = u2-u0;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

        tangents.insert(tangents.end(), {tangent,tangent,tangent});
        bitangents.insert(bitangents.end(), {bitangent,bitangent,bitangent});
    }
    for(int i=0; i<tangents.size(); i++)
    {
        tangents[i] = glm::normalize(tangents[i]);
    }
}
void print(vector<float> pos, vector<float> tex, vector<uint> indicies);


struct point {
  GLfloat x;
  GLfloat y;
};

GLuint VBO = 6;
point vertices[101][101];


void MeshResourceExtraData::parseObjectFile(char *rawBuffer, int size)
{
    cout<<"MeshResourceExtraData parseObjectFile"<<endl;
#ifdef TINY_OBJ_LOADER_H_
//    // assume mtl is in the same folder as the obj
//        std::string mtl_basepath = m_meshFileName;
//        size_t last_slash = mtl_basepath.find_last_of("/");
//        if (last_slash == std::string::npos)
//            mtl_basepath = "./";
//        else
//            mtl_basepath = mtl_basepath.substr(0, last_slash + 1);

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    if(!tinyobj::LoadObj(shapes,materials,err, rawBuffer,size,"", tinyobj::calculate_normals|tinyobj::triangulation))
    {
        cout<<"tinyobject::LoadObj from rawbuffer  error: "<<err<<endl;
        return;
    }
    if(!err.empty())
    {
        cout<<"tinyobject::LoadObj from rawbuffer warning: "<<err<<endl;
    }
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
    cout<<"Mesh Loaded"<<endl;

    for(const auto &shape : shapes)
    {
        cout<<"begin shape read cycle"<<endl;
        const tinyobj::mesh_t mesh = shape.mesh;
        computeTanBitangents(mesh,tangents,bitangents);
        //MeshNode newMesh;
        //m_meshFileName = shape.name;
        m_Mesh.indexCount = mesh.indices.size();
        m_Mesh.vertexCount = mesh.positions.size()/3;

        //print(mesh.positions, mesh.texcoords, mesh.indices);

        cout << "index count = " << m_Mesh.indexCount<<endl;
        cout << "tex count = "<< mesh.texcoords.size()/2<<endl;
        cout << "tangents count" << tangents.size()<<endl;
        GLuint newPositionBO;
        glGenBuffers(1, &newPositionBO);
        glBindBuffer(GL_ARRAY_BUFFER, newPositionBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>((mesh.positions.size()/3) * sizeof(glm::vec3)),
                mesh.positions.data(), GL_STATIC_DRAW);
        m_Mesh.positionBO = static_cast<GLushort>(newPositionBO);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        cout << "Pos buf = "<< mesh.positions.size()/3 * sizeof(glm::vec3)<<endl;

        GLuint newTexCoordBO;
        glGenBuffers(1,&newTexCoordBO);
        glBindBuffer(GL_ARRAY_BUFFER, newTexCoordBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>((mesh.texcoords.size()/2) * sizeof(glm::vec2)),
                mesh.texcoords.data(), GL_STATIC_DRAW);
        m_Mesh.texCoordBO = static_cast<GLushort>(newTexCoordBO);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        cout<<"Tex buf = "<<mesh.texcoords.size()/2 * sizeof(glm::vec2)<<endl;

        GLuint newNormalsBO;
        glGenBuffers(1,&newNormalsBO);
        glBindBuffer(GL_ARRAY_BUFFER, newNormalsBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(mesh.normals.size()/3*sizeof(glm::vec3)),
                mesh.normals.data(), GL_STATIC_DRAW);
        m_Mesh.normalBO = static_cast<GLushort>(newNormalsBO);
        glBindBuffer(GL_ARRAY_BUFFER,0);

        GLuint newTangentsBO;
        glGenBuffers(1,&newTangentsBO);
        glBindBuffer(GL_ARRAY_BUFFER, newTangentsBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(tangents.size()*sizeof(glm::vec3)),
                tangents.data(), GL_STATIC_DRAW);
        m_Mesh.tangentBO = static_cast<GLushort>(newTangentsBO);
        glBindBuffer(GL_ARRAY_BUFFER,0);

        GLuint newIndexBo;
        glGenBuffers(1,&newIndexBo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newIndexBo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(mesh.indices.size() * sizeof(unsigned)),
                mesh.indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        m_Mesh.indexBO = static_cast<GLushort>(newIndexBo);
        cout<<"Index buf = "<<mesh.indices.size() * sizeof(unsigned)<<endl;

    }


    /* Add materials */
    //Code....
    //........
    /////

    /*Alternative to download object file with objloaderfile */
    {
#elif
#include "objloader.h"
        vector<glm::vec3> Pos;
        vector<glm::vec2> Tex;
        vector<glm::vec3> Norm;
        vector<glm::vec3> OutPos;
        vector<glm::vec2> OutTex;
        vector<glm::vec3> OutNorm;
        vector<unsigned short> OutIndicies;
        loadObj(filename.c_str(),Pos, Tex, Norm);
        indexVBO(Pos,Tex,Norm,OutIndicies, OutPos,OutTex,OutNorm);

        Mesh newMesh;
        newMesh.name = "earth";
        newMesh.indexCount = OutIndicies.size();
        newMesh.vertexCount = Pos.size();

        GLuint newPositionBO;
        glGenBuffers(1, &newPositionBO);
        glBindBuffer(GL_ARRAY_BUFFER, newPositionBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(OutPos.size() * sizeof(glm::vec3)),
                                OutPos.data(), GL_STATIC_DRAW);
        newMesh.positionBO = newPositionBO;
        glBindBuffer(GL_ARRAY_BUFFER,0);
        cout << "Pos buf = "<<OutPos.size() * sizeof(glm::vec3)<<endl;

        GLuint newTexCoordBO;
        glGenBuffers(1,&newTexCoordBO);
        glBindBuffer(GL_ARRAY_BUFFER, newTexCoordBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(OutTex.size() * sizeof(glm::vec2)),
                                OutTex.data(), GL_STATIC_DRAW);
        newMesh.texCoordBO = newTexCoordBO;
        glBindBuffer(GL_ARRAY_BUFFER,0);
        cout << "Tex buf = "<<OutTex.size() * sizeof(glm::vec2)<<endl;

        GLuint newNormalsBO;
        glGenBuffers(1,&newNormalsBO);
        glBindBuffer(GL_ARRAY_BUFFER, newNormalsBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(OutNorm.size() * sizeof(glm::vec3)),
                                OutNorm.data(), GL_STATIC_DRAW);
        newMesh.normalBO = newNormalsBO;
        glBindBuffer(GL_ARRAY_BUFFER,0);

        GLuint newIndexBo;
        glGenBuffers(1,&newIndexBo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newIndexBo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(OutIndicies.size() * sizeof(unsigned)),
                                OutIndicies.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        newMesh.indexBO = newIndexBo;
        cout << "Index buf = "<<OutIndicies.size() * sizeof(unsigned)<<endl;

    //       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.indexBO);
    //        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

        GLuint newMeshId = scene.meshes.insert(newMesh);
        meshIDs.push_back(newMeshId);

        cout<<OutPos.size()*3<<endl;
        cout << OutTex.size()*2 << endl;
        for(size_t i=0; i<OutPos.size(); i++)
        {
            cout <<OutPos[i].x << " "<< OutPos[i].y <<" "<<OutPos[i].z<< endl;
        }
        cout << endl << endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
        for(size_t i=0; i<OutTex.size(); i++)
        {
            cout <<OutTex[i].x << " "<< OutTex[i].y << endl;
        }
        for(size_t i=0; i<OutIndicies.size();i+=3)
        {
            cout<<OutIndicies[i]<<" "<<OutIndicies[i+1]<<" "<<OutIndicies[i+2]<<endl;
        }
    }
#endif
    }
}

bool MeshResourceLoader::vLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle)
{
    if(rawSize <= 0)
    {
        GCC_ASSERT(false && "MeshResourceLoader load Resource with size < 0");
        return false;
    }
    shared_ptr<MeshResourceExtraData> pExtraData = shared_ptr<MeshResourceExtraData>(GCC_NEW MeshResourceExtraData());
    pExtraData->parseObjectFile(rawBuffer,rawSize);
    handle->setExtra(shared_ptr<MeshResourceExtraData>(pExtraData));
    return true;
}

std::shared_ptr<IResourceLoader> createMeshObjectResourceLoader()
{
    return shared_ptr<IResourceLoader>(GCC_NEW MeshResourceLoader());
}

unsigned int MeshResourceLoader::vGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize)
{
    // The raw data of the Mesh file is needed by the CDXUTMesh class, so we're going to keep it around.
    return rawSize;
}


float MeshNode::calcBoundingSphere()
{
//    float radius = 0.0f;
//    for( UINT subset = 0; subset < mesh11->GetNumSubsets( 0 ); ++subset )
//    {
//		Vec3 extents = mesh11->GetMeshBBoxExtents(subset);
//		extents.x = abs(extents.x);
//		extents.y = abs(extents.y);
//		extents.z = abs(extents.z);
//		radius = (radius > extents.x) ? radius : extents.x;
//		radius = (radius > extents.y) ? radius : extents.y;
//		radius = (radius > extents.z) ? radius : extents.z;
//	}
//	return radius;
}

HRESULT MeshNode::vOnRestore(Scene *pScene)
{
    V_RETURN(SceneNode::vOnRestore(pScene));
    //V_RETURN(vertexShader.onRestore(pScene));
    //V_RETURN(m_PixelShader.onRestore(pScene));

    //meshShader.init();

    //Force the Mesh to reload
//    Resource resource(m_sdkMeshFileName);
//    shared_ptr<ResHandle> pResourceHandle = g_pApp->m_ResCache->GetHandle(&resource);
//	shared_ptr<D3DSdkMeshResourceExtraData11> extra = static_pointer_cast<D3DSdkMeshResourceExtraData11>(pResourceHandle->GetExtra());

//	SetRadius(CalcBoundingSphere(&extra->m_Mesh11));
    return S_OK;
}


//HRESULT MeshNode::vOnUpdate(Scene *pScene, const DWORD elapsedMs)
//{

//}

HRESULT MeshNode::vRender(Scene *pScene)
{
    cout<<"MeshNode vRender"<<endl;
    meshShader.Use();
    meshShader.setupRender(pScene,this);

    //Get the Mesh
    Resource resource(m_meshFileName);
    shared_ptr<ResHandle> pResourceHandle = g_pApp->m_resCache->getHandle(&resource);
    shared_ptr<MeshResourceExtraData> extra = static_pointer_cast<MeshResourceExtraData>(pResourceHandle->getExtra());
    extra->m_Mesh.debugPrintMesh();
    glBindBuffer(GL_ARRAY_BUFFER, extra->m_Mesh.positionBO);
    glVertexAttribPointer(SCENE_POSITION_ATTRIB_LOCATION,3,GL_FLOAT,GL_FALSE,sizeof(glm::vec3),nullptr);
    glEnableVertexAttribArray(SCENE_POSITION_ATTRIB_LOCATION);

    glBindBuffer(GL_ARRAY_BUFFER, extra->m_Mesh.texCoordBO);
    glVertexAttribPointer(SCENE_TEXCOORD_ATTRIB_LOCATION,2,GL_FLOAT,GL_FALSE,sizeof(glm::vec2),nullptr);
    glEnableVertexAttribArray(SCENE_TEXCOORD_ATTRIB_LOCATION);

    glBindBuffer(GL_ARRAY_BUFFER, extra->m_Mesh.normalBO);
    glVertexAttribPointer(SCENE_NORMAL_ATTRIB_LOCATION,3,GL_FLOAT,GL_FALSE,sizeof(glm::vec3),nullptr);
    glEnableVertexAttribArray(SCENE_NORMAL_ATTRIB_LOCATION);

    glBindBuffer(GL_ARRAY_BUFFER, extra->m_Mesh.tangentBO);
    glVertexAttribPointer(SCENE_TANGENT_ATTRIB_LOCATION,3,GL_FLOAT,GL_FALSE,sizeof(glm::vec3),nullptr);
    glEnableVertexAttribArray(SCENE_TANGENT_ATTRIB_LOCATION);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, extra->m_Mesh.indexBO);
    glDrawElements(GL_TRIANGLES,static_cast<GLsizei>(extra->m_Mesh.indexCount),GL_UNSIGNED_INT, nullptr);


    //Draw grid lines
    /*
    for(int i = 0; i < 101; i++) {
      for(int j = 0; j < 101; j++) {
        vertices[i][j].x = (j - 50) / 50.0;
        vertices[i][j].y = (i - 50) / 50.0;
      }
    }

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    for(int i = 0; i < 101; i++) {
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 101 * sizeof(point), (void *)(i * sizeof(point)));
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINE_STRIP, 0, 101);
    }
    */

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    return S_OK;
}

void MeshNode::setupShader(const char *vertexShaderName, const char *fragmentShaderName)
{
    meshShader.setPath(vertexShaderName, fragmentShaderName);
}

void MeshNode::setupShader(const Shader & shader)
{
    meshShader = shader;
}

//HRESULT MeshNode::vPick(Scene *pScene, RayCast *pRayCast)
//{
//	if (SceneNode::VPick(pScene, pRayCast)==E_FAIL)
//		return E_FAIL;

//	pScene->PushAndSetMatrix(m_Props.ToWorld());

//    //Get the Mesh
//	Resource resource(m_sdkMeshFileName);
//    shared_ptr<ResHandle> pResourceHandle = g_pApp->m_ResCache->GetHandle(&resource);
//	shared_ptr<D3DSdkMeshResourceExtraData11> extra = static_pointer_cast<D3DSdkMeshResourceExtraData11>(pResourceHandle->GetExtra());

//	HRESULT hr = pRayCast->Pick(pScene, m_Props.ActorId(), &extra->m_Mesh11);
//	pScene->PopMatrix();

//	return hr;
//}

void print(vector<float> pos, vector<float> tex, vector<uint> indicies)
{
    cout<<pos.size() << endl;
    cout << tex.size() << endl;
    for(size_t i=0; i<pos.size(); i+=3)
    {
        cout << pos[i] << " "<< pos[i+1] << " "<<pos[i+2] << endl;
    }
    cout << endl << endl<< endl << endl<< endl << endl<< endl << endl<< endl << endl<< endl << endl<< endl << endl<< endl << endl;
    for(size_t i=0; i<tex.size(); i+=2)
    {
        cout << tex[i] << " "<< tex[i+1] << endl;
    }
    cout << endl << endl<< endl << endl<< endl << endl<< endl << endl<< endl << endl<< endl << endl<< endl << endl<< endl << endl;
    for(size_t i=0; i<indicies.size(); i+=3)
        cout<<indicies[i]<<" "<<indicies[i+1]<<" "<<indicies[i+2]<<endl;
    cout<<endl;
}





