#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
 
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
 
#include <vector>
#include <iostream>
#include <fstream>
 
#include "load_model_meshes.h"
#include "shader_configure.h"

void framebuffer_size_callback(GLFWwindow* , int , int );
void displayParts(Model & );

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
 
int main()
{
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

	glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);
 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	// PARA WINDOWS

	// const char* vert_shader = "../OwnProjects/Transformer/Shaders/shader_glsl.vert";
	// const char* frag_shader = "../OwnProjects/Transformer/Shaders/shader_glsl.frag";

	// PARA MAC

	const char* vert_shader = "/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/Shaders/shader_glsl.vert";
	const char* frag_shader = "/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/Shaders/shader_glsl.frag";
 
	Shader main_shader(vert_shader, frag_shader);
	main_shader.use();	
	
	unsigned int view_matrix_loc = glGetUniformLocation(main_shader.ID, "view");
	unsigned int projection_matrix_loc = glGetUniformLocation(main_shader.ID, "projection");
	unsigned int camera_position_loc = glGetUniformLocation(main_shader.ID, "camera_position");
 
	// glm::vec3 camera_position(0.0f, 30.0f, 0.0f); // -Z is into the screen.		
	// glm::vec3 camera_target(0.0f, 0.0f, 0.0f);
	// glm::vec3 camera_up(0.0f, 0.0f, -1.0f);

	glm::vec3 camera_position(-20.0f, 5.0f, 40.0f); // -Z is into the screen.		
	glm::vec3 camera_target(0.0f, 0.0f, 0.0f);
	glm::vec3 camera_up(0.0f, 1.0f, 0.0f);
 
	glUniform3f(camera_position_loc, camera_position.x, camera_position.y, camera_position.z);
    glm::mat4 view(1.0f);
	view = glm::lookAt(camera_position, camera_target, camera_up);
	glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, &view[0][0]);
    glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, &projection[0][0]);
 
	srand((unsigned)time(NULL));
	float spin_speed = (float)(4);
    
    float x_angle = 0.0f; // Initialize rotation angle for X axis
	glm::mat4 spinning_mat(1.0f);

	glm::mat4 identidad(1.0f);
 
	unsigned int animate_loc = glGetUniformLocation(main_shader.ID, "animate");

	//PARA WINDOWS

	//  Model cabeza("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\cabeza.obj");
    //  Model abdomen("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\abdomen.obj");
    //  Model pecho("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\pecho.obj");
	//  Model antebrazo1("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\antebrazo1.obj");
    //  Model antebrazo2("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\antebrazo2.obj");
	//  Model hombro1("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\hombro1.obj");
    //  Model hombro2("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\hombro2.obj");
    //  Model mano1("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\mano1.obj");
	//  Model mano2("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\mano2.obj");
	//  Model cadera("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\cadera.obj");
	//  Model pierna1("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\pierna1.obj");
	//  Model pierna2("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\pierna2.obj");
	//  Model pantorrilla1("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\pantorrilla1.obj");
	//  Model pantorrilla2("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\pantorrilla2.obj");
	//  Model pie1("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\pie1.obj");
	//  Model pie2("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\pie2.obj");
	//  Model rueda1("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\rueda1.obj");
	//  Model rueda2("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\rueda2.obj");
	//  Model rueda3("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\rueda3.obj");
	//  Model rueda4("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\rueda4.obj");

	//  Model road("E:\\LabCompiler2023_II_CG\\glfw-master\\OwnProjects\\Transformer\\OBJS\\road.obj");

	// PARA MAC

	Model cabeza("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/cabeza.obj");
    Model abdomen("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/abdomen.obj");
    Model pecho("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/pecho.obj");
	Model antebrazo1("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/antebrazo1.obj");
    Model antebrazo2("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/antebrazo2.obj");
	Model hombro1("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/hombro1.obj");
    Model hombro2("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/hombro2.obj");
    Model mano1("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/mano1.obj");
	Model mano2("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/mano2.obj");
	Model cadera("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/cadera.obj");
	Model pierna1("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/pierna1.obj");
	Model pierna2("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/pierna2.obj");
	Model pantorrilla1("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/pantorrilla1.obj");
	Model pantorrilla2("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/pantorrilla2.obj");
	Model pie1("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/pie1.obj");
	Model pie2("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/pie2.obj");
	Model rueda1("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/rueda1.obj");
	Model rueda2("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/rueda2.obj");
	Model rueda3("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/rueda3.obj");
	Model rueda4("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/rueda4.obj");

	Model road("/Volumes/KINGSTON/LabCompiler2023_II_CG/glfw-master/OwnProjects/Transformer/OBJS/road.obj");

	SceneGraph * grafo = new SceneGraph();

	SceneNode * cabezaSG = new SceneNode(&cabeza);
	SceneNode * abdomenSG = new SceneNode(&abdomen);
	SceneNode * pechoSG = new SceneNode(&pecho);
	SceneNode * antebrazo1SG = new SceneNode(&antebrazo1);
	SceneNode * antebrazo2SG = new SceneNode(&antebrazo2);
	SceneNode * hombro1SG = new SceneNode(&hombro1);
	SceneNode * hombro2SG = new SceneNode(&hombro2);
	SceneNode * mano1SG = new SceneNode(&mano1);
	SceneNode * mano2SG = new SceneNode(&mano2);
	SceneNode * caderaSG = new SceneNode(&cadera);
	SceneNode * pierna1SG = new SceneNode(&pierna1);
	SceneNode * pierna2SG = new SceneNode(&pierna2);
	SceneNode * pantorrilla1SG = new SceneNode(&pantorrilla1);
	SceneNode * pantorrilla2SG = new SceneNode(&pantorrilla2);
	SceneNode * pie1SG = new SceneNode(&pie1);
	SceneNode * pie2SG = new SceneNode(&pie2);
	SceneNode * rueda1SG = new SceneNode(&rueda1);
	SceneNode * rueda2SG = new SceneNode(&rueda2);
	SceneNode * rueda3SG = new SceneNode(&rueda3);
	SceneNode * rueda4SG = new SceneNode(&rueda4);

	SceneNode * roadSG = new SceneNode(&road);

	grafo->addChild(pechoSG);
	grafo->addChild(cabezaSG);
	grafo->addChild(caderaSG);

	pechoSG->addChild(hombro1SG);
	pechoSG->addChild(abdomenSG);
	pechoSG->addChild(hombro2SG);

	hombro1SG->addChild(antebrazo1SG);
	hombro2SG->addChild(antebrazo2SG);

	antebrazo1SG->addChild(mano1SG);
	antebrazo2SG->addChild(mano2SG);

	caderaSG->addChild(pierna1SG);
	caderaSG->addChild(pierna2SG);

	pierna1SG->addChild(rueda1SG);
	pierna1SG->addChild(pantorrilla1SG);

	pantorrilla1SG->addChild(rueda2SG);
	pantorrilla1SG->addChild(pie1SG);

	pierna2SG->addChild(rueda3SG);
	pierna2SG->addChild(pantorrilla2SG);

	pantorrilla2SG->addChild(rueda4SG);
	pantorrilla2SG->addChild(pie2SG);
 
	glActiveTexture(GL_TEXTURE0);
	unsigned int image_sampler_loc = glGetUniformLocation(main_shader.ID, "image");
	glUniform1i(image_sampler_loc, 0);

	glUniformMatrix4fv(animate_loc, 1, GL_FALSE, glm::value_ptr(identidad));
	glm::vec3 pivotPoint(-0.0f, 0.0f, -0.0f);

	glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 translationVector = glm::vec3(-0.0041555f, 0.1733625f, 0.0f);
    model = glm::translate(model, translationVector);
	glm::vec3 scaleVector = glm::vec3(3.0f, 3.0f, 8.0f);
    model = glm::scale(model, scaleVector);
	translationVector = glm::vec3(0.0f, -3.0f, 0.0f);
	model = glm::translate(model, translationVector);

	spinning_mat = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	grafo->transform(spinning_mat);
	roadSG->transform(model);
 
 
 
	//MOVEMENT


	bool forward= false;
	float fwrd = 0.01;
	float max_fwrd = 40.0;
	
	
	bool backward= false;
	float bwrd = 0.01;
	float max_bwrd = 40.0;
	
	//Animacion
	float maximun = 5.00f;
	
	bool truck = true;
	float max_head = 2.0f * maximun;
	
	float head = 0.01;
	bool head1 = true;
	
	float max_hands = 1.5f * maximun;
	float hands = 0.01;
	bool hphase1 = false;

	float max_arms = 2.35f * maximun;
	float arms = 0.01;	
	bool aphase1 = false, aphase2 = false,aphase3 = false;
	
	float max_feet = 1.2f * maximun;
	float feet = 0.01;
	bool feets = false;
	
	float max_wheels = 1.8f * maximun;
	float wheels = 0.01;
	bool wheels1 = false;
	
	int max_chst =900;
	int chst =1;
	bool chest = false;
	
	int max_hips = 900;
	int hips = 1;
	bool hips1 = false;
	spin_speed = 0.1;
	
	float max_hips2 =0.9f * maximun;
	float hps2= 0.01;
	bool hips2= false;
	
	float max_wheels2 = 2.0f * maximun;
	float wls2 = 0.01;
	bool wheels2 = false;
	
	float max_wheels3 = 0.5f * maximun;
	float wls3 = 0.01;
	bool wheels3 = false;
	
	bool optimus = false;
	
	bool mb_wheels = false;

	float offset = 0.01* maximun;
 
	while (!glfwWindowShouldClose(window))
	{
		
		if(truck){
			if(head1){
				if(max_head <= head){
					head1 = false;
					hphase1 = true;
					head = offset;
				}
				
				glm::mat4 head_d = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.01f, 0.0f));
		
				head += offset;
				cabezaSG->transform(head_d);
		
		
			}
			
			if(hphase1){
				if(max_hands <= hands){
					hphase1 = false;
					aphase1 = true;
					hands = offset;
				}
				glm::mat4 hands_up = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.01f, 0.0f));
				hands += offset;
				mano1SG->transform(hands_up);
				mano2SG->transform(hands_up);
			}
			
			
			if(aphase1){
				if(max_arms <= arms){
					aphase1 = false;
					aphase2 = true;
					max_arms = 2.35f * maximun;
					arms = offset;
					
				}
				glm::mat4 arms_back = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.01f));
				arms += offset;
				hombro1SG->transform(arms_back);
				hombro2SG->transform(arms_back);
			}
			
			if(aphase2){
				if(max_arms <= arms){
					aphase2 = false;
					feets = true;
					arms =offset;
				}
				glm::mat4 arms_in1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.01f, 0.0f, 0.0f));
				glm::mat4 arms_in2 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.01f, 0.0f, 0.0f));
				arms += offset;
				hombro1SG->transform(arms_in1);
				hombro2SG->transform(arms_in2);
			}
			
			
			
			if(feets){
				if(max_feet <= feet){
					feets = false;
					wheels1 = true;
					feet =offset;
				}
				glm::mat4 feet_in = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.01f));

				feet += offset;
				pie1SG->transform(feet_in);
				pie2SG->transform(feet_in);
			}
			
		
			if(wheels1){
				if(max_wheels <= wheels){
					wheels1 = false;
					chest = true; /////
					wheels =offset;
				}
				glm::mat4 wheel1_u = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.01f, 0.0f));
				wheels += offset;
				rueda1SG->transform(wheel1_u);
				rueda3SG->transform(wheel1_u);
				
				//std:: cout << "offset: "<<wheels << endl;
			}


			
			if(chest){
				
				pivotPoint[0] = 0.0f;
				pivotPoint[1] = 0.0f;
				pivotPoint[2] = 0.0f;
				
				if(max_chst <= chst){
					chest = false;
					hips1 = true; 
					chst =1;
				}
				
				glm::mat4 pivotTransform = glm::translate(glm::mat4(1.0f), pivotPoint);
				pivotTransform = glm::rotate(pivotTransform, glm::radians(spin_speed), glm::vec3(-1.0f, .0f, 0.0f));
				pivotTransform = glm::translate(pivotTransform, -pivotPoint);
				pechoSG->transform(pivotTransform);
				cabezaSG->transform(pivotTransform);
				chst += 1;
				
				//std:: cout << "offset: "<<chst << endl;
			}
			


			if (hips1){
				
				pivotPoint[0] = 0.0f;
				pivotPoint[1] = -7.0f;
				pivotPoint[2] = 0.0f;
				
				if(max_hips <= hips){
					hips1 = false;
					hips2= true; /////
					hips =1;
				}
				
				glm::mat4 pivotTransform = glm::translate(glm::mat4(1.0f), pivotPoint);
				pivotTransform = glm::rotate(pivotTransform, glm::radians(spin_speed), glm::vec3(1.0f, .0f, 0.0f));
				pivotTransform = glm::translate(pivotTransform, -pivotPoint);
				grafo->transform(pivotTransform);
				hips += 1;
				//std:: cout << "offset: "<<hips << endl;
			}



			
			
			if (hips2){

				if(max_hips2 <= hps2){
					hips2 = false;
					wheels2= true; /////
					hps2 =offset;
				}
				
				glm::mat4 pecho_a = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.01f, 0.0f));
				hps2 += offset;
				pechoSG->transform(pecho_a);
				cabezaSG->transform(pecho_a);
				
				//std:: cout << "offset: "<<hps2 << endl;
			}
			
			if (wheels2){

				if(max_wheels2 <= wls2){
					wheels2 = false;
					wheels3 = true;
					wls2 = offset;
				}
				glm::mat4 wheels_b = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.01f));
				wls2 += offset;
				rueda2SG->transform(wheels_b);
				rueda4SG->transform(wheels_b);
				
				//std:: cout << "offset: "<<wls2 << endl;
			}
			
			if (wheels3){

				if(max_wheels3 <= wls3){
					
					truck=false;
					forward = true;
					//optimus = true;
					
					wls3 = offset;
				}
				glm::mat4 wheels_b1 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.01f, 0.0f, 0.0f));
				glm::mat4 wheels_b2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.01f, 0.0f, 0.0f));
				wls3 += offset;
				rueda2SG->transform(wheels_b1);
				rueda4SG->transform(wheels_b2);
				

			}
			
			
		}
		
		
		if(forward){
			if (max_fwrd <= fwrd){
				backward  = true;
				forward = false;
				fwrd = offset;
			}
			fwrd += offset;
			glm::mat4 adelante = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.01f));
			grafo->transform(adelante);
			//std:: cout << "offset: "<<fwrd << endl;
		}
		
		
		if(backward){
			if (max_bwrd <= bwrd){
				backward=false;
				optimus  = true;
				bwrd = offset;
			}
			
			bwrd += offset;
			glm::mat4 atras = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.01f));
			grafo->transform(atras);
			//std:: cout << "offset: "<<bwrd << endl;
		}
		
		
		//sleep()
		if(optimus){
			if (wheels3){
	
				if(max_wheels3 <= wls3){
					wheels3 = false;
					wheels2 = true;
					wls3 = offset;
				}
				glm::mat4 wheels_b1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.01f, 0.0f, 0.0f));
				glm::mat4 wheels_b2 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.01f, 0.0f, 0.0f));
				wls3 += offset;
				rueda2SG->transform(wheels_b1);
				rueda4SG->transform(wheels_b2);
				
			

			}
			
			if (wheels2){
				
				if(max_wheels2 <= wls2){
					wheels2 = false;
					hips2 = true;
					wls2 = offset;
				}
				glm::mat4 wheels_b = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.01f));
				wls2 += offset;
				rueda2SG->transform(wheels_b);
				rueda4SG->transform(wheels_b);
				

			}
			
			if (hips2){
				
				if(max_hips2 <= hps2){
					hips2 = false;
					hips1= true; /////
					hps2 = offset;
				}

				glm::mat4 pecho_a = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.01f, 0.0f));
				hps2 += offset;
				pechoSG->transform(pecho_a);
				cabezaSG->transform(pecho_a);
				

			}
			
			if (hips1){
				
				pivotPoint[0] = 0.0f;
				pivotPoint[1] = -7.0f;
				pivotPoint[2] = 0.0f;
				
				if(max_hips <= hips){
					hips1 = false;
					chest= true; /////
					hips =1;
				}
				
				glm::mat4 pivotTransform = glm::translate(glm::mat4(1.0f), pivotPoint);
				pivotTransform = glm::rotate(pivotTransform, glm::radians(spin_speed), glm::vec3(-1.0f, .0f, 0.0f));
				pivotTransform = glm::translate(pivotTransform, -pivotPoint);
				grafo->transform(pivotTransform);
				hips += 1;

			}
			
			
			
						
			if(chest){
				
				pivotPoint[0] = 0.0f;
				pivotPoint[1] = 0.0f;
				pivotPoint[2] = 0.0f;
				
				if(max_chst <= chst){
					chest = false;
					wheels1 = true; 
					chst =1;
				}
				
				glm::mat4 pivotTransform = glm::translate(glm::mat4(1.0f), pivotPoint);
				pivotTransform = glm::rotate(pivotTransform, glm::radians(spin_speed), glm::vec3(1.0f, .0f, 0.0f));
				pivotTransform = glm::translate(pivotTransform, -pivotPoint);
				pechoSG->transform(pivotTransform);
				cabezaSG->transform(pivotTransform);
				chst += 1;
				
				//std:: cout << "offset: "<<chst << endl;
			}
			
			
			
			if(wheels1){
				if(max_wheels <= wheels){
					wheels1 = false;
					feets= true; /////
					wheels =offset;
				}
				glm::mat4 wheell_u = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.01f, 0.0f));
				wheels += offset;
				rueda1SG->transform(wheell_u);
				rueda3SG->transform(wheell_u);

			}
			
			
			if(feets){
				if(max_feet <= feet){
					feets = false;
					aphase2 = true;
					feet =offset;
				}
				glm::mat4 feet_in = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.01f));
				
				feet += offset;
				pie1SG->transform(feet_in);
				pie2SG->transform(feet_in);
			}
			
			if(aphase2){
				if(max_arms <= arms){
					aphase2 = false;
					aphase1 = true;
					arms = offset;
				}
				glm::mat4 arms_in1 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.01f, 0.0f, 0.0f));
				glm::mat4 arms_in2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.01f, 0.0f, 0.0f));
				arms += offset;
				hombro1SG->transform(arms_in1);
				hombro2SG->transform(arms_in2);
			}
			
			if(aphase1){
				if(max_arms <= arms){
					aphase1 = false;
					hphase1 = true;
					max_arms = 2.35f * maximun;
					arms = offset;
					
				}
				glm::mat4 arms_back = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.01f));
				arms += offset;
				hombro1SG->transform(arms_back);
				hombro2SG->transform(arms_back);
			}
			
			
			
			
			if(hphase1){
				if(max_hands <= hands){
					hphase1 = false;
					head1 = true;
					hands = offset;
				}
				glm::mat4 hands_up = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.01f, 0.0f));
				hands += offset;
				mano1SG->transform(hands_up);
				mano2SG->transform(hands_up);
			}
			
			
			if(head1){
				if(max_head <= head){
					//head1 = false;
					//hphase1 = true;
					truck=true;
					optimus=false;
					head = offset;
				}
				
				glm::mat4 head_d = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.01f, 0.0f));
				
				head += offset;
				cabezaSG->transform(head_d);
				
				
			}
		}
		
		
		if (mb_wheels){
			
				glm::vec3 pivotPoint2(-0.5f, 0.0f, -0.0f);
			
				glm::mat4 pivotTransform = glm::translate(glm::mat4(1.0f), pivotPoint);
				glm::mat4 pivotTransform2 = glm::translate(glm::mat4(1.0f), pivotPoint2);
				
				
				pivotTransform = glm::rotate(pivotTransform, glm::radians(spin_speed), glm::vec3(-1.0f, .0f, 0.0f));
				pivotTransform = glm::translate(pivotTransform, -pivotPoint);
				
				pivotTransform2 = glm::rotate(pivotTransform2, glm::radians(spin_speed), glm::vec3(-0.5f, .0f, 0.0f));
				pivotTransform2 = glm::translate(pivotTransform2, -pivotPoint2);
				
				
				
				
				rueda2SG->transform(pivotTransform);
				rueda4SG->transform(pivotTransform2);
				hips += 1;
				
			//std:: cout << "offset: "<<hips << endl;
		}

		glClearColor(0.30f, 0.55f, 0.65f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		grafo->draw();
		
		roadSG->draw();
		//animation();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(main_shader.ID);
 
	glfwTerminate();
 
	exit(EXIT_SUCCESS);
}

void displayParts(Model & modelo){
	for (unsigned int i = 0; i < modelo.num_meshes; ++i)
	{			
		glBindTexture(GL_TEXTURE_2D, modelo.mesh_list[i].tex_handle); // Bind texture for the current mesh.	

		glBindVertexArray(modelo.mesh_list[i].VAO);
		glDrawElements(GL_TRIANGLES, (GLsizei)modelo.mesh_list[i].vert_indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}