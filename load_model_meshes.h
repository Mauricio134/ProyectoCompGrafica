#pragma once

using namespace std;

class Model
{
private:
	Assimp::Importer importer;
	const aiScene* scene = nullptr;
	aiNode* root_node = nullptr;
	
	struct Mesh
	{
		unsigned int VAO, VBO1, VBO2, VBO3, EBO;
		
		vector<glm::vec3> vert_positions;
		vector<glm::vec3> vert_normals;
		vector<glm::vec2> tex_coords;
		vector<unsigned int> vert_indices;
		unsigned int tex_handle;
	};	
 
	struct Texture
	{
		unsigned int textureID;
		string image_name;
	};

public:
	unsigned int num_meshes;
	vector<Mesh> mesh_list;
	vector<Texture> texture_list;
 
	Model(const char* model_path)
	{
		scene = importer.ReadFile(model_path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs);	
 
		load_model();
	}

	void transformar(glm::mat4 matrizTransformador){
		for (int i = 0; i < mesh_list.size(); ++i) {
			for (int j = 0; j < mesh_list[i].vert_positions.size(); ++j) {
				glm::vec3 vertex = mesh_list[i].vert_positions[j];
				glm::vec4 temp_vert(vertex.x, vertex.y, vertex.z, 1.0f);
				temp_vert = matrizTransformador * temp_vert;
				mesh_list[i].vert_positions[j] = glm::vec3(temp_vert);
			}
			for (int j = 0; j < mesh_list[i].vert_normals.size(); ++j) {
				glm::vec3 vertex = mesh_list[i].vert_normals[j];
				glm::vec4 temp_vert(vertex.x, vertex.y, vertex.z, 1.0f);
				glm::mat4 normalMatrix = glm::mat3(glm::transpose(glm::inverse(matrizTransformador)));
				temp_vert = normalMatrix * temp_vert;
				mesh_list[i].vert_normals[j] = glm::vec3(temp_vert);
			}
			update_buffer_data(i);
		}
	}

private:	
	void load_model()
	{
		if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
			cout << "Assimp importer.ReadFile (Error) -- " << importer.GetErrorString() << "\n";
		else
		{
			num_meshes = scene->mNumMeshes;
			mesh_list.resize(num_meshes);
 
			aiMesh* mesh{};		
			int indices_offset = 0;
			for (unsigned int i = 0; i < num_meshes; ++i)
			{
				mesh = scene->mMeshes[i];			
 
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
 
				for (unsigned int tex_count = 0; tex_count < material->GetTextureCount(aiTextureType_DIFFUSE); ++tex_count)
				{
					aiString string;
					material->GetTexture(aiTextureType_DIFFUSE, tex_count, &string);

					int already_loaded = is_image_loaded(string.C_Str());
 
					if (already_loaded == -1)
					{
						bool load_success = false;
						unsigned int texture_handle = load_texture_image(string.C_Str(), load_success);
 
						if (load_success)
						{
							Texture texture;
							texture.image_name = string.C_Str();
							texture.textureID = texture_handle;
 
							texture_list.push_back(texture);
							mesh_list[i].tex_handle = texture_handle;
						}
					}
					else	
						mesh_list[i].tex_handle = already_loaded;
				}


				for (unsigned int i2 = 0; i2 < mesh->mNumVertices; ++i2)
				{
						glm::vec3 position{};
						position.x = mesh->mVertices[i2].x;
						position.y = mesh->mVertices[i2].y;
						position.z = mesh->mVertices[i2].z;
						mesh_list[i].vert_positions.push_back(position);					
 
						if (mesh->HasNormals())
						{
							glm::vec3 normal{};
							normal.x = mesh->mNormals[i2].x;
							normal.y = mesh->mNormals[i2].y;
							normal.z = mesh->mNormals[i2].z;
							mesh_list[i].vert_normals.push_back(normal);
						}
						else
							mesh_list[i].vert_normals.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
 
						if (mesh->HasTextureCoords(0))
						{
							glm::vec2 tex_coords{};
							tex_coords.x = mesh->mTextureCoords[0][i2].x;
							tex_coords.y = mesh->mTextureCoords[0][i2].y;							
							mesh_list[i].tex_coords.push_back(tex_coords);							
						}
						else
							mesh_list[i].tex_coords.push_back(glm::vec2(0.0f, 0.0f));
				}


				for (unsigned int i3 = 0; i3 < mesh->mNumFaces; ++i3)
					for (unsigned int i4 = 0; i4 < mesh->mFaces[i3].mNumIndices; ++i4)										
						mesh_list[i].vert_indices.push_back(mesh->mFaces[i3].mIndices[i4] + indices_offset);
 
				set_buffer_data(i);
			}			
		}
	}
 
	void set_buffer_data(unsigned int index)
	{
		glGenVertexArrays(1, &mesh_list[index].VAO);
		glGenBuffers(1, &mesh_list[index].VBO1);
		glGenBuffers(1, &mesh_list[index].VBO2);
		glGenBuffers(1, &mesh_list[index].VBO3);
		glGenBuffers(1, &mesh_list[index].EBO);
 
		glBindVertexArray(mesh_list[index].VAO);		

		glBindBuffer(GL_ARRAY_BUFFER, mesh_list[index].VBO1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh_list[index].vert_positions.size(), &mesh_list[index].vert_positions[0], GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, mesh_list[index].VBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh_list[index].vert_normals.size(), &mesh_list[index].vert_normals[0], GL_STATIC_DRAW);
 
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, mesh_list[index].VBO3);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh_list[index].tex_coords.size(), &mesh_list[index].tex_coords[0], GL_STATIC_DRAW);
 
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_list[index].EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh_list[index].vert_indices.size(), &mesh_list[index].vert_indices[0], GL_STATIC_DRAW);
 
		glBindVertexArray(0);
	}
 
	int is_image_loaded(string file_name)
	{		
		for (unsigned int i = 0; i < texture_list.size(); ++i)	
			if (file_name.compare(texture_list[i].image_name) == 0)
				return texture_list[i].textureID;
		return -1;
	}
 
	unsigned int load_texture_image(string file_name, bool& load_complete)
	{	
 
		file_name = "/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/Images/" + file_name;		
 
		int width, height, num_components;
		unsigned char* image_data = stbi_load(file_name.c_str(), &width, &height, &num_components, 0);
 
		unsigned int textureID;
		glGenTextures(1, &textureID);		
 
		if (image_data)
		{
			GLenum format{};
 
			if (num_components == 1)
				format = GL_RED;
			else if (num_components == 3)
				format = GL_RGB;
			else if (num_components == 4)
				format = GL_RGBA;
 
			glBindTexture(GL_TEXTURE_2D, textureID);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
 
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image_data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
			load_complete = true;
			stbi_image_free(image_data);			
			cout << "   Image loaded OK: " << file_name << "\n";
		}
		else
		{
			load_complete = false;
			stbi_image_free(image_data);
			cout << "   Image failed to load: " << file_name << "\n";
		}
		return textureID;
	}

	void update_buffer_data(unsigned int index){
		glBindVertexArray(mesh_list[index].VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, mesh_list[index].VBO1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh_list[index].vert_positions.size(), &mesh_list[index].vert_positions[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, mesh_list[index].VBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh_list[index].vert_normals.size(), &mesh_list[index].vert_normals[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, mesh_list[index].VBO3);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh_list[index].tex_coords.size(), &mesh_list[index].tex_coords[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_list[index].EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh_list[index].vert_indices.size(), &mesh_list[index].vert_indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
};

class SceneNode{
private:
	Model * modelo;
	vector<SceneNode *> children;
	bool isroot;
public:
	SceneNode(): isroot(true) {}
	SceneNode(Model * m) : modelo(m), isroot(false){}
	~SceneNode(){
		delete modelo;
		for(int i = 0; i < children.size(); i++){
			delete children[i];
		}
	}

	void addChild(SceneNode * nodo){ children.push_back(nodo); }

	void draw(){
		if(!isroot){
			for (unsigned int i = 0; i < modelo->num_meshes; ++i)
			{			
				glBindTexture(GL_TEXTURE_2D, modelo->mesh_list[i].tex_handle);	

				glBindVertexArray(modelo->mesh_list[i].VAO);
				glDrawElements(GL_TRIANGLES, (GLsizei)modelo->mesh_list[i].vert_indices.size(), GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}
		for(int i = 0; i < children.size(); i++){
			children[i]->draw();
		}
	}

	void transform(glm::mat4 matrizTransformador){
		if(!isroot) modelo->transformar(matrizTransformador);
		for(int i = 0; i < children.size(); i++){
			children[i]->transform(matrizTransformador);
		}
	}
};

class SceneGraph {
private:
	SceneNode* root;
public:

    SceneGraph() {
        root = new SceneNode();
    }

    ~SceneGraph() {
        delete root;
    }

    void draw() {
        root->draw();
    }

	void transform(glm::mat4 matrizTransformador) {
        root->transform(matrizTransformador);
    }

	void addChild(SceneNode * node){
		root->addChild(node);
	}
};