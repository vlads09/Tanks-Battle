#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;
using namespace implemented;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::RenderTank(float deltaTimeSeconds) {
    
    // Render the tank's body
    glm::mat4 modelMatrixBody = glm::mat4(1.0f);
    modelMatrixBody = glm::translate(modelMatrixBody, tank->body_pos);
    modelMatrixBody = glm::rotate(modelMatrixBody, tank->rotate_body, 
        glm::vec3(0, 1, 0));
    modelMatrixBody = glm::scale(modelMatrixBody, tank->scale);
    RenderSimpleMesh(meshes["body"], shaders["LabShader"], modelMatrixBody, 
        glm::vec3(0.035, 0.741, 0));
    glm::vec3 cameraOffset = glm::vec3(0, 3, -5.5f);  // Initial camera offset from the tank
    glm::vec3 tankForward = glm::rotate(glm::mat4(1.0f), tank->rotate_body, glm::vec3(0, 1, 0)) * 
        glm::vec4(0, 0, 1, 0);
    glm::vec3 cameraPosition = tank->body_pos + tankForward * cameraOffset.z + 
        glm::vec3(0, cameraOffset.y, 0);
    glm::vec3 lookAtPosition = tank->body_pos + tankForward;

    camera->Set(cameraPosition, lookAtPosition, glm::vec3(0, 1, 0));
    // Render the turret (relative to the body)
    glm::mat4 modelMatrixTurret = modelMatrixBody; // Initialize with body's matrix
    modelMatrixTurret = glm::translate(modelMatrixTurret, tank->turret_pos);
    modelMatrixTurret = glm::rotate(modelMatrixTurret, tank->rotate_turret, 
        glm::vec3(0, 1, 0));

    RenderSimpleMesh(meshes["turret"], shaders["LabShader"], modelMatrixTurret,
        glm::vec3(0.392, 0.569, 0.384));

    // Render the cannon (relative to the turret or body, adjust as needed)
    glm::mat4 modelMatrixCannon = modelMatrixTurret; // Initialize with turret's matrix
    modelMatrixCannon = glm::translate(modelMatrixCannon, tank->cannon_pos);

    RenderSimpleMesh(meshes["cannon"], shaders["LabShader"], modelMatrixCannon, 
        glm::vec3(0.231, 0.231, 0.231));

    for (int i = 0; i < tank->ammos.size(); i++) 
    {
        if (tank->ammos[i].shoot) 
        {
            tank->ammos[i].initialPosition = tank->ammos[i].position;
            tank->ammos[i].matrix = modelMatrixCannon;
            tank->ammos[i].position += glm::vec3(0, 0.3, 1);
            tank->ammos[i].matrix = glm::translate(tank->ammos[i].matrix, 
                tank->ammos[i].position);
            tank->ammos[i].matrix = glm::scale(tank->ammos[i].matrix, 
                glm::vec3(0.2, 0.2, 0.2));
            tank->ammos[i].shoot = false;
        }
        else {
            tank->ammos[i].position += glm::vec3(0, 0, 0.0002 * deltaTimeSeconds);
            tank->ammos[i].matrix = glm::translate(tank->ammos[i].matrix, 
                tank->ammos[i].position);
        }
        if (tank->ammos[i].position.z - tank->ammos[i].initialPosition.z > 1.0004)
        {
            tank->ammos.erase(tank->ammos.begin() + i);
        }
        else 
        {
            RenderSimpleMesh(meshes["ammo"], shaders["LabShader"], tank->ammos[i].matrix, 
                glm::vec3(1, 1, 1));
        }
        
    }
}

void Tema2::RenderEnemies(float deltaTimeSeconds)
{
    for (int i = 0; i < enemies.size(); i++)
    {
        Tank* tank_e = &enemies[i];
        glm::mat4 modelMatrixBody = glm::mat4(1.0f);
        modelMatrixBody = glm::translate(modelMatrixBody, 
            tank_e->body_pos + glm::vec3(0, 0, 2 * move));
        modelMatrixBody = glm::rotate(modelMatrixBody, tank_e->rotate_body, glm::vec3(0, 1, 0));
        modelMatrixBody = glm::scale(modelMatrixBody, tank_e->scale);
        RenderSimpleMesh(meshes["body"], shaders["VertexNormal"], modelMatrixBody, 
            glm::vec3(1, 0.525, 0));

        // Render the turret (relative to the body)
        glm::mat4 modelMatrixTurret = modelMatrixBody; // Initialize with body's matrix
        modelMatrixTurret = glm::translate(modelMatrixTurret, tank_e->turret_pos);
        modelMatrixTurret = glm::rotate(modelMatrixTurret, tank_e->rotate_turret, 
            glm::vec3(0, 1, 0));

        RenderSimpleMesh(meshes["turret"], shaders["VertexNormal"], modelMatrixTurret, 
            glm::vec3(0.392, 0.569, 0.384));

        // Render the cannon (relative to the turret or body, adjust as needed)
        glm::mat4 modelMatrixCannon = modelMatrixTurret; // Initialize with turret's matrix
        modelMatrixCannon = glm::translate(modelMatrixCannon, tank_e->cannon_pos);

        RenderSimpleMesh(meshes["cannon"], shaders["VertexNormal"], modelMatrixCannon, 
            glm::vec3(0.231, 0.231, 0.231));
    }
}

void Tema2::Init()
{
    renderCameraTarget = false;
    time = glfwGetTime();
    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    {
        Mesh* mesh = new Mesh("body");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), 
            "body.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cannon");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), 
            "cannon.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("turret");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tank"), 
            "turret.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("ammo");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), 
            "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plain");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"),
            "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, 
            "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, 
            "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    glm::vec3 pos_enemy = glm::vec3(2, 0, 0);
    for (int i = 0; i < 4; i++) {
        Tank* tank = new Tank();
        enemies.push_back(*tank);
        enemies[i].body_pos += glm::vec3(pos_enemy.x * 2 * (i + 1) + 5, 0, 0);
    }
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::RenderPlain()
{
    glm::mat4 modelMatrixPlain = glm::mat4(1.0f);
    modelMatrixPlain = glm::translate(modelMatrixPlain, glm::vec3(0, 0.02f, 0));
    modelMatrixPlain = glm::scale(modelMatrixPlain, glm::vec3(1, 1, 1));
    RenderSimpleMesh(meshes["plain"], shaders["LabShader"], modelMatrixPlain, 
        glm::vec3(0.349, 0.259, 0.161));
}


void Tema2::Update(float deltaTimeSeconds)
{
    if (glfwGetTime() - time >= 60)
    {
        // end the game
        exit(0);
    }
    RenderPlain();
    RenderTank(deltaTimeSeconds);
    RenderEnemies(move);
    if (move <= 7 && move >= 0)
    {
        if (direction == 0)
        {
            // move forward
            move += deltaTimeSeconds;
        }
        else
        {
            // move backwards
            move -= deltaTimeSeconds;
        }
    }
    else if (move > 7)
    {
        direction = 1;
        move -= deltaTimeSeconds;
    }
    else
    {
        direction = 0;
        move += deltaTimeSeconds;
    }
}


void Tema2::FrameEnd()
{
    
}


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light & material properties
    // Set light position uniform

    glm::vec3 eyePosition = camera->position;
    // Set eye position (camera position) uniform
    GLint eyePositionLocation = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(eyePositionLocation, 1, glm::value_ptr(eyePosition));
    //glUniform3f(eyePositionLocation, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    GLint material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, 0);

    GLint material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, 0);

    GLint material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, 0);

    // Set the color of the object
    GLint object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    mesh->Render();
    // lDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    
    
    if (window->KeyHold(GLFW_KEY_W)) {
        tank->forward = glm::normalize(glm::rotate(glm::mat4(1.0f), tank->rotate_body, 
            glm::vec3(0, 1, 0)) * glm::vec4(glm::vec3(0, 0, 1), 1));
        glm::vec3 dir = glm::normalize(glm::vec3(tank->forward.x, 0, tank->forward.z));
        tank->body_pos += dir * (2 * deltaTime);
        if (window->KeyHold(GLFW_KEY_D)) {
            tank->rotate_body -= deltaTime;

        }
        if (window->KeyHold(GLFW_KEY_A)) {
            tank->rotate_body += deltaTime;

        }
            
    }
    else if (window->KeyHold(GLFW_KEY_S)) {
        // TODO(student): Translate the camera forward
        tank->forward = glm::normalize(glm::rotate(glm::mat4(1.0f), tank->rotate_body, 
            glm::vec3(0, 1, 0)) * glm::vec4(glm::vec3(0, 0, 1), 1));
        glm::vec3 dir = glm::normalize(glm::vec3(tank->forward.x, 0, tank->forward.z));
        tank->body_pos -= dir * (2 * deltaTime);
        if (window->KeyHold(GLFW_KEY_D)) {
            tank->rotate_body += deltaTime;

        }
        if (window->KeyHold(GLFW_KEY_A)) {
            tank->rotate_body -= deltaTime;

        }
            
    }
    else if (window->KeyHold(GLFW_KEY_D)) {
        tank->rotate_body -= deltaTime;
            
    }
    else if (window->KeyHold(GLFW_KEY_A)) {
        tank->rotate_body += deltaTime;
            
    }

}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    float sensitivity = 0.1f;

    if (mouseX > mouseMoveX) {
        tank->rotate_turret -= sensitivity;
        mouseMoveX = mouseX;
    }
    else if (mouseX < mouseMoveX) {
        tank->rotate_turret += sensitivity;
        mouseMoveX = mouseX;
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_2) {
        Ammo* ammo = new Ammo();
        ammo->position = tank->cannon_pos - glm::vec3(0, 0, tank->cannon_pos.z);
        tank->ammos.push_back(*ammo);
    }
    
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
