
#define WINDOW_TITLE "Gravity Nodes" // Name of the window
#define WINDOW_WIDTH 1600 //  Self explanatory
#define WINDOW_HEIGHT 900 // Self explanatory

#define NODE_MASS 100 // Mass of the node
#define NODE_RADIUS 10 // Radius of the node
#define NODE_SPEED 50 // Node speed
#define WORLD_SIZE 0 //  Dont modify, doesnt work but might cause memory leak
#define WORLD_SIZE_MAX 100000 // Increase if you want more nodes. Node id depends on this.
#define GRAVITATIONAL_CONSTANT 10 // Self explanatory
#define MULTIPLIER 10 // Speed multiplier

#define USE_SHADER // Comment out if you dont want shaders. Doesnt work rn. Absolutely doesnt work in export mode but wait for me to do it even tho i wont.
#define SHADER "cross_hatching" // Name of shader file. Look at `res` folder. Only fragment shader files.

#define EXPORT // Comment out this for interactive mode. All the below params are ignored if this is commente.
#define EXPORT_FPS 60 // Self explanatory
#define EXPORT_TIME_SEC 10 // Length of video
#define EXPORT_CREATE_NODES_EVERY 1 // Creates nodes every n seconds
#define EXPORT_DEFAULT_WORLD_SIZE 10 // Creates n nodes upon world creation
