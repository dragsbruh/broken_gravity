#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <config.h>

#ifdef USE_SHADER
Shader shader;
#endif

RenderTexture2D target;

typedef struct {
    Vector2 pos;
    Vector2 vel;
    Color color;
    float mass;
    int id;
} Node;

typedef struct {
    Node *nodes;
    int size;
} World;

void node_update(Node *node, World *world, float dt) {
    for (int i = 0; i < world->size; i++) {
        Node *other = &world->nodes[i];
        if (other->id == node->id) {
            continue;
        }
        float distance_x = other->pos.x - node->pos.x;
        float distance_y = other->pos.y - node->pos.y;
        float distance = sqrt(distance_x * distance_x + distance_y * distance_y);
        float force = ((GRAVITATIONAL_CONSTANT * node->mass * other->mass) / (distance * distance));
        float angle = atan2(distance_x, distance_y);
        node->vel.x += cos(angle) * force;
        node->vel.y += sin(angle) * force;
    }
    if (node->pos.x < NODE_RADIUS) {
        node->pos.x = NODE_RADIUS;
        node->vel.x = -node->vel.x;
    } else if (node->pos.x > WINDOW_WIDTH - NODE_RADIUS) {
        node->pos.x = WINDOW_WIDTH - NODE_RADIUS;
        node->vel.x = -node->vel.x;
    }
    if (node->pos.y < NODE_RADIUS) {
        node->pos.y = NODE_RADIUS;
        node->vel.y = -node->vel.y;
    } else if (node->pos.y > WINDOW_HEIGHT - NODE_RADIUS) {
        node->pos.y = WINDOW_HEIGHT - NODE_RADIUS;
        node->vel.y = -node->vel.y;
    }
    node->pos.x += node->vel.x * dt * dt * MULTIPLIER;
    node->pos.y += node->vel.y * dt * dt * MULTIPLIER;
    DrawCircle(node->pos.x, node->pos.y, NODE_RADIUS, node->color);
}

void world_init(World *world) {
    world->nodes = malloc(sizeof(Node) * WORLD_SIZE);
    world->size = WORLD_SIZE;

    for (int i = 0; i < WORLD_SIZE; i++) {
        Node *node = &world->nodes[i];
        node->pos = (Vector2) {
            (float) rand() / (float) WINDOW_WIDTH,
            (float) rand() / (float) WINDOW_HEIGHT
        };
        node->vel = (Vector2) {0, 0};
        node->color = (Color) {
            (float) rand() / (float) 255,
            (float) rand() / (float) 255,
            (float) rand() / (float) 255
        };
    }
}

void world_update(World *world, float dt) {
    for (int i = 0; i < world->size; i++) {
        Node *node = &world->nodes[i];
        node_update(node, world, dt);
    }
}

void world_create_node(World *world, Node node) {
    world->size++;
    world->nodes = realloc(world->nodes, world->size * sizeof(Node));
    world->nodes[world->size - 1] = node;
    world->nodes[world->size - 1].id = rand() % WORLD_SIZE_MAX;
    world->nodes[world->size - 1].mass = NODE_MASS;
}

void update(World *world, float dt) {
    BeginTextureMode(target);
    ClearBackground(BLACK);
    #ifdef USE_SHADER
    BeginShaderMode(shader);
    #endif
    
    world_update(world, dt); // draws some shapes

    #ifdef USE_SHADER
    EndShaderMode();
    #endif
    DrawText(TextFormat("Nodes: %d", world->size), 10, 10, 10, WHITE);
    EndTextureMode();
}

void Interactive() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    World world;
    world_init(&world);
    world_create_node(&world, (Node){(Vector2){WINDOW_WIDTH/2, WINDOW_HEIGHT/2}, (Vector2){0,0}, WHITE});

    #ifdef USE_SHADER
    shader = LoadShader(0, TextFormat("res/%s.fs", SHADER));
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    #endif

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            world_create_node(&world, (Node){GetMousePosition(), (Vector2){0, 0}, RED});
        }

        update(&world, dt);
        
        BeginDrawing();
        DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2){ 0, 0 }, WHITE);
        EndDrawing();
    }

    #ifdef USE_SHADER
    UnloadShader(shader);
    #endif
    UnloadRenderTexture(target);
    CloseWindow();
}

void Exportable() {
    InitWindow(800, 200, TextFormat("Export: %s", WINDOW_TITLE));
    
    World world;
    world_init(&world);
    
    #ifdef USE_SHADER
    shader = LoadShader(0, TextFormat("res/%s.fs", SHADER));
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    #endif
    
    float dt = (float)1/(float)EXPORT_FPS;
    int finishedFrames = 0;
    int targetNumFrames = EXPORT_FPS * EXPORT_TIME_SEC;

    for (int i = 0; i < EXPORT_DEFAULT_WORLD_SIZE; i++) {
        world_create_node(&world, (Node){
                (Vector2){
                    (float) rand() / (float) WINDOW_WIDTH,
                    (float) rand() / (float) WINDOW_HEIGHT
                },
                (Vector2){0, 0},
                WHITE
            });
    }

    while (finishedFrames < targetNumFrames && !WindowShouldClose()) {
        update(&world, dt);
        finishedFrames++;
        
        BeginDrawing();
        ClearBackground(BLACK);
        

        DrawText(TextFormat("Export Framerate: %d\n", EXPORT_FPS), 10, 10, 20, WHITE);
        DrawText(TextFormat("Export frames: %d\n", targetNumFrames), 10, 30, 20, WHITE);
        DrawText(TextFormat("Export video length: %d\n", EXPORT_TIME_SEC), 10, 50, 20, WHITE);
        DrawText(TextFormat("Export progress: %d/%d", finishedFrames,targetNumFrames), 10, 80, 40, WHITE);
        EndDrawing();
        Image image = LoadImageFromTexture(target.texture);
        ExportImage(image, TextFormat("frames\\%d.png", finishedFrames));
        if (finishedFrames % (EXPORT_FPS * EXPORT_CREATE_NODES_EVERY) == 0) {
            world_create_node(&world, (Node){
                (Vector2){
                    (float) rand() / (float) WINDOW_WIDTH,
                    (float) rand() / (float) WINDOW_HEIGHT
                },
                (Vector2){0, 0},
                WHITE
            });
        }
        UnloadImage(image);
    }

    #ifdef USE_SHADER
    UnloadShader(shader);
    #endif
    UnloadRenderTexture(target);
    CloseWindow();
}

int main(int argc, char **argv) {
    SetTraceLogLevel(LOG_WARNING);
    #ifdef EXPORT
    Exportable();
    #else
    Interactive();
    #endif
    return 0;
}