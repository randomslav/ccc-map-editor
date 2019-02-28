#include "map_editor.h"

int main(){
    MapEditor app;
    if(app.Construct(768, 480, 2, 2))
    //if(app.Construct(1280, 720, 1, 1))
    //if(app.Construct(1536, 960, 1, 1))
        app.Start();

    return 0;
}
