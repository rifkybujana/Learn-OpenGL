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

    /* Initialize GLAD */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return -1;
    }

    /* Set viewport and callback */
    glViewport(0, 0, 600, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* Use stack allocation for objects to avoid memory leaks */
    App app;
    Shader shader;
    Object3D obj;

    /* Setup ImGui context */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    /* Set ImGui style */
    ImGui::StyleColorsDark();

    /* Initialize ImGui backends */
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(shader.version);

    /* Generate and bind VAO, VBO, EBO outside the render loop */
    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    /* Setup vertex attributes */
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    /* Define vertex layout */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* Unbind VAO to prevent accidental modification */
    glBindVertexArray(0);

    /* Render loop */
    while (!glfwWindowShouldClose(window))
    {
        /* Input handling */
        ProcessInput(window);

        /* Update object data */
        obj.GetCorner();

        /* Update buffer data */
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, obj.vertices.size() * sizeof(float), obj.vertices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.indices.size() * sizeof(unsigned int), obj.indices.data(), GL_DYNAMIC_DRAW);

        /* Rendering commands */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader.shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(obj.indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        /* Start ImGui frame */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /* ImGui interface */
        ImGui::Begin("Circle Settings");
        ImGui::SliderInt("Total Angle", &obj.circleCorner, 4, 30);
        ImGui::End();

        ImGui::Begin("Vertices");
        for (size_t i = 0; i < obj.vertices.size(); i += 3) {
            ImGui::Text("%.2f, %.2f, %.2f", obj.vertices[i], obj.vertices[i + 1], obj.vertices[i + 2]);
        }
        ImGui::End();

        ImGui::Begin("Indices");
        for (size_t i = 0; i < obj.indices.size(); i += 3) {
            ImGui::Text("%u, %u, %u", obj.indices[i], obj.indices[i + 1], obj.indices[i + 2]);
        }
        ImGui::End();

        /* Render ImGui */
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap buffers and poll events */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /* Cleanup ImGui */
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    /* De-allocate resources */
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader.shaderProgram);

    /* Terminate GLFW */
    glfwTerminate();

    return 0;
}

void ProcessInput(GLFWwindow* window) {
    /* Close window on ESC press */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    /* Adjust the viewport when the window size changes */
    glViewport(0, 0, width, height);
}
