#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAX 100

// Stack data structure
typedef struct {
    int top;
    double items[MAX];
} Stack;

// Stack operations
void initStack(Stack *stack) {
    stack->top = -1;
}

int isEmpty(Stack *stack) {
    return stack->top == -1;
}

void push(Stack *stack, double value) {
    if (stack->top < MAX - 1) {
        stack->items[++stack->top] = value;
    } else {
        printf("Stack overflow!\n");
    }
}

double pop(Stack *stack) {
    if (!isEmpty(stack)) {
        return stack->items[stack->top--];
    } else {
        printf("Stack underflow!\n");
        return -1;  // Return a sentinel value
    }
}

double peek(Stack *stack) {
    if (!isEmpty(stack)) {
        return stack->items[stack->top];
    }
    return -1; // Return a sentinel value if stack is empty
}

// Function to return precedence of operators
int precedence(char operator) {
    if (operator == '+' || operator == '-') {
        return 1;
    } else if (operator == '*' || operator == '/') {
        return 2;
    }
    return 0;  // For '('
}

// Function to perform arithmetic operations
double applyOperator(double a, double b, char operator) {
    switch (operator) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default: return 0;
    }
}

// Function to convert infix to postfix
void infixToPostfix(char* infix, char* postfix) {
    Stack stack;
    initStack(&stack);
    int k = 0;
    int i;
    for ( i = 0; infix[i] != '\0'; i++) {
        char token = infix[i];

        // If the token is a number (operand), add it to the postfix expression
        if (isdigit(token)) {
            postfix[k++] = token;
        }
        // If the token is an opening parenthesis, push it to the stack
        else if (token == '(') {
            push(&stack, token);
        }
        // If the token is a closing parenthesis, pop from stack until opening parenthesis is found
        else if (token == ')') {
            while (!isEmpty(&stack) && peek(&stack) != '(') {
                postfix[k++] = pop(&stack);
            }
            pop(&stack);  // Remove the '(' from the stack
        }
        // If the token is an operator, pop operators from the stack with higher or equal precedence
        else if (token == '+' || token == '-' || token == '*' || token == '/') {
            while (!isEmpty(&stack) && precedence(peek(&stack)) >= precedence(token)) {
                postfix[k++] = pop(&stack);
            }
            push(&stack, token);
        }
    }

    // Pop all the remaining operators in the stack
    while (!isEmpty(&stack)) {
        postfix[k++] = pop(&stack);
    }

    postfix[k] = '\0'; // Null terminate the postfix expression
}

// Function to evaluate a postfix expression
double evaluatePostfix(char* postfix) {
    Stack stack;
    initStack(&stack);
    int i;
    for ( i = 0; postfix[i] != '\0'; i++) {
        char token = postfix[i];

        // If the token is a number, push it to the stack
        if (isdigit(token)) {
            push(&stack, token - '0');  // Convert char to int
        }
        // If the token is an operator, pop operands and apply the operator
        else if (token == '+' || token == '-' || token == '*' || token == '/') {
            double b = pop(&stack);
            double a = pop(&stack);
            double result = applyOperator(a, b, token);
            push(&stack, result);
        }
    }

    return pop(&stack);  // Final result is the only element in the stack
}

int main() {
    char infix[MAX], postfix[MAX];

    printf("Enter an infix expression: ");
    fgets(infix, sizeof(infix), stdin);

    // Remove newline character from the input
    infix[strcspn(infix, "\n")] = 0;

    infixToPostfix(infix, postfix);
    printf("Postfix Expression: %s\n", postfix);

    double result = evaluatePostfix(postfix);
    printf("Result: %.2f\n", result);

    return 0;
}