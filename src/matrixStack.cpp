#include "matrixStack.h"

#include <iostream>

MatrixStack::MatrixStack(){
    stack.push(glm::mat4(1.0f));
}

void MatrixStack::Push(){
    stack.push(stack.top());
}

void MatrixStack::Pop(){
    if (IsEmpty())
    {
        std::cerr << "The matrix stack is empty!!!" << std::endl;
        return;
    }
    stack.pop();
}

void MatrixStack::Save(const glm::mat4 &matrix){
    stack.pop();
    stack.push(matrix);
}
bool MatrixStack::IsEmpty(){
    return stack.empty();
}

glm::mat4 MatrixStack::Top(){
    return stack.top();
}
