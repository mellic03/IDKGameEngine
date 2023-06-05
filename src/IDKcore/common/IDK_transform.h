#pragma once

#include "IDKdecl.h"


class idk::transform
{
private:
    idk::vec4 _position;
    idk::vec4 _scale;

public:
                        transform() {  };
    void                modelMatrix();
    void                modelMatrixStale();

};

