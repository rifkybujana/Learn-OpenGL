#include "App.h"
#include "Object3D.h"
#include "shader.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void ProcessInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
    glfwInit();

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(600, 600, "Learn OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 600, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    App* app = new App();
    Shader* shader = new Shader();
    Object3D* obj = new Object3D();

    // Setup ImGui 
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // set imgui style
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(shader->version);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input    
        ProcessInput(window);

        obj->GetCorner();

        // ----------------------- RENDERING COMMAND HERE ----------------------------- //
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, obj->vertices.size() * sizeof(float), &obj->vertices[0], GL_STATIC_DRAW);

        unsigned int EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->indices.size() * sizeof(unsigned int), &obj->indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shader->shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, obj->indices.size() * sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

        // --------------------------- IMGUI ----------------------------------------- //
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame(); {

            // render your GUI
            ImGui::Begin("Circle Settings");
            ImGui::SliderInt("Total Angle", &obj->circleCorner, 4, 30);
            ImGui::End();

            ImGui::Begin("Vertices");

            std::string str;

            for (int i = 0; i < obj->vertices.size(); i += 3) {
                std::ostringstream ost;
                std::copy(obj->vertices.begin() + i, obj->vertices.begin() + i + 3, std::ostream_iterator<float>(ost, ",\t"));

                str += ost.str();
                str += '\n';
            }

            const char* c = str.c_str();

            ImGui::Text(c);
            ImGui::End();

            ImGui::Begin("Indices");

            str = "";

            for (int i = 0; i < obj->indices.size(); i += 3) {
                std::ostringstream ost;
                std::copy(obj->indices.begin() + i, obj->indices.begin() + i + 3, std::ostream_iterator<int>(ost, ",\t"));

                str += ost.str();
                str += '\n';
            }

            c = str.c_str();

            ImGui::Text(c);
            ImGui::End();
        }

        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteProgram(shader->shaderProgram);

    glfwTerminate();

    return 0;
}

void ProcessInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}