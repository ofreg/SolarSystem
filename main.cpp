#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h> // Для роботи з GLUT

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>












// Функції завантаження шейдерів та їх компіляції
std::string loadShaderSource(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Функція для компіляції шейдера
GLuint compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // Перевірка на помилки компіляції
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* log = new char[length];
        glGetShaderInfoLog(shader, length, &length, log);
        std::cerr << "Shader compile error: " << log << std::endl;
        delete[] log;
    }
    return shader;
}

// Функція для лінкування шейдерів в програму
GLuint createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Перевірка на помилки лінкування
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLint length;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
        char* log = new char[length];
        glGetProgramInfoLog(shaderProgram, length, &length, log);
        std::cerr << "Program link error: " << log << std::endl;
        delete[] log;
    }

    // Видалення окремих шейдерів після лінкування
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// Функція для малювання сфери (Сонце або планета)
// Функція для малювання сфери (Сонце або планета)
/*void drawSphere(float x, float y, float z, float radius) {
    glPushMatrix(); // Зберігаємо поточну трансформацію
    glTranslatef(x, y, z); // Переміщаємо об'єкт у відповідне місце

    // Налаштування матеріалу для червоної сфери
    GLfloat mat_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };  // Червоний колір для сфери
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);  // Встановлюємо матеріал для дифузного освітлення
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_diffuse); // Для спекулярного відбиття

    GLfloat mat_shininess[] = { 50.0f };  // Гладкість матеріалу
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Увімкнення освітлення
    glEnable(GL_LIGHTING);  // Увімкнення освітлення
    glEnable(GL_LIGHT0);    // Увімкнення першого джерела світла

    // Налаштування позиції джерела світла
    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f }; // Позиція джерела світла
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); // Встановлення позиції джерела світла

    // Налаштування кольору джерела світла
    GLfloat light_color[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Біле світло
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color); // Налаштування дифузного освітлення

    // Малюємо сферу
    glutSolidSphere(radius, 20, 20);

    // Вимкнення освітлення після малювання
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);

    glPopMatrix(); // Відновлюємо трансформацію
}*/
float earthAngle = 0.0f;

// Функція для анімації обертання планет

// Функція для малювання сонця
// Функція для малювання Сонця і налаштування світла
void drawSun(float x, float y, float z, float radius) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // Налаштування параметрів світла (Sonne як точкове джерело світла)
    GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };  // Позиція світла
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 0.3f, 1.0f };   // Яскраве жовте світло
    GLfloat light_specular[] = { 1.0f, 1.0f, 0.0f, 1.0f };  // Спекулярне світло
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };   // М'яке навколишнє світло

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);  // Встановлюємо позицію світла
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);    // Дифузне світло
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);  // Спекулярне світло
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);    // Амбієнтне світло
    glEnable(GL_LIGHT0); // Увімкнути світло

    // Матеріали для Сонця (для візуалізації)
    GLfloat mat_diffuse[] = { 1.0f, 1.0f, 0.0f, 1.0f };  // Жовтий колір
    GLfloat mat_specular[] = { 1.0f, 1.0f, 0.0f, 1.0f }; // Спекулярне світло
    GLfloat mat_emission[] = { 1.0f, 1.0f, 0.3f, 1.0f }; // Випромінювання
    GLfloat mat_shininess[] = { 80.0f };

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);    // Дифузний компонент
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);  // Спекулярний компонент
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);  // Випромінювання
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// Гладкість

    // Малюємо Сонце (сферу)
    glutSolidSphere(radius, 30, 30); // Розмір і сегменти сфери

    glPopMatrix();
}


void drawPlanet(float x, float y, float z, float radius) {
    glPushMatrix();  // Зберігаємо поточну трансформацію
    glTranslatef(x, y, z);  // Переміщаємо планету

    // Матеріал для планети
    GLfloat mat_diffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f };   // Синій колір
    GLfloat mat_specular[] = { 0.5f, 0.5f, 1.0f, 1.0f }; // Блиск
    GLfloat mat_emission[] = { 0.0f, 0.0f, 0.3f, 1.0f }; // Легке світіння
    GLfloat mat_shininess[] = { 80.0f };  // Гладкість

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);    // Дифузний компонент
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);  // Спекулярний компонент
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);  // Випромінювання
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// Гладкість поверхні

    // Малюємо сферу для планети
    glutSolidSphere(radius, 20, 20);

    glPopMatrix();  // Відновлюємо трансформацію
}


void animatePlanet(float& angle, float radius, float speed) {
    angle += speed;  // Збільшуємо кут на задану швидкість
    if (angle > 360.0f) {
        angle -= 360.0f;  // Якщо перевищено 360 градусів, скидаємо до 0
    }

    glPushMatrix();
    glRotatef(angle, 0.0f, 1.0f, 0.0f);  // Обертання планети навколо Сонця
    glTranslatef(radius, 0.0f, 0.0f);    // Позиціюємо планету на орбіті
    drawPlanet(0.0f, 0.0f, 0.0f, 0.5f);  // Малюємо планету (використовуємо радіус 0.5)

    drawPlanet(2.5f, 0.0f, 0.0f, 0.5f);
    //  drawPlanet(2.5f, 0.5f, 0.0f, 0.4f);
    glPopMatrix();
}

void setupLighting() {
    /**/glEnable(GL_LIGHTING);   // Увімкнення освітлення
    glEnable(GL_LIGHT0);     // Увімкнення першого джерела світла

    // Позиція світла (точкове джерело світла в точці 0, 0, 0)
    GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };  // Точкове світло
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Параметри світла
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };   // Дифузне світло (біле)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // Спекулярне світло (яскраві відблиски)
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };   // Амбієнтне світло (загальне освітлення)
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

    glEnable(GL_DEPTH_TEST);  // Увімкнення тесту глибини*/
}



void drawLine() {
    glPushMatrix();

    // Вимикаємо освітлення, щоб використовувати кольори без його впливу
    glDisable(GL_LIGHTING);

    // Додамо обертання сцени/
    //////////////////////////////
    glRotatef(45.0f, 1.0f, 1.0f, 0.0f);  // Обертання на 45 градусів навколо осі (1,1,0)
    //////////////////////////////
    // Малюємо першу лінію (від -5 до 5 по осі X)
    glColor3f(1.0f, 1.0f, 0.0f);  // Жовтий колір
    glBegin(GL_LINES);
    glVertex3f(-5.0f, 0.0f, 0.0f);
    glVertex3f(5.0f, 0.0f, 0.0f);
    glEnd();

    // Малюємо другу лінію (від -5 до 5 по осі Y)
    glColor3f(0.0f, 1.0f, 1.0f);  // Бірюзовий колір
    glBegin(GL_LINES);
    glVertex3f(0.0f, -5.0f, 0.0f);
    glVertex3f(0.0f, 5.0f, 0.0f);
    glEnd();

    // Малюємо третю лінію (від -5 до 5 по осі Z)
    glColor3f(1.0f, 0.0f, 1.0f);  // Фіолетовий колір
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, -5.0f);  // Точка початку лінії
    glVertex3f(0.0f, 0.0f, 5.0f);   // Точка кінця лінії
    glEnd();

    // Повертаємо освітлення назад
    glEnable(GL_LIGHTING);

    glPopMatrix();
}
float cameraPosX = 0.0f, cameraPosY = 0.0f, cameraPosZ = -10.0f;  // Початкові координати камери
float cameraSpeed = 0.1f;  // Швидкість руху камери

// Функція для обробки вводу з клавіатури
void keyboard(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') {
        cameraPosZ += cameraSpeed;  // Переміщуємо камеру вперед
    }
    if (key == 's' || key == 'S') {
        cameraPosZ -= cameraSpeed;  // Переміщуємо камеру назад
    }
    if (key == 'a' || key == 'A') {
        cameraPosX -= cameraSpeed;  // Переміщуємо камеру вліво
    }
    if (key == 'd' || key == 'D') {
        cameraPosX += cameraSpeed;  // Переміщуємо камеру вправо
    }

    glutPostRedisplay();  // Перемалювати сцену після зміни позиції камери
}
void drawRectangle(float x, float y, float z, float width, float height) {
    glPushMatrix();
    glTranslatef(x, y, z);  // Переміщення на задані координати

    // Малювання прямокутника (квадрат)
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f); // Колір прямокутника (червоний)
    glVertex3f(-width / 2, -height / 2, 0.0f); // Нижній лівий кут
    glVertex3f(width / 2, -height / 2, 0.0f);  // Нижній правий кут
    glVertex3f(width / 2, height / 2, 0.0f);   // Верхній правий кут
    glVertex3f(-width / 2, height / 2, 0.0f);  // Верхній лівий кут
    glEnd();

    glPopMatrix();
}

int main(int argc, char** argv) {
    // Ініціалізація GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Solar System");

    // Ініціалізація GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);

    // Налаштування освітлення
    setupLighting();
    glutKeyboardFunc(keyboard);
    // Основний цикл рендерингу
    while (true) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();
        // Переміщаємо камеру так, щоб вона була над сценою
        glTranslatef(0.0f, 0.0f, -10.0f);  // Камера знаходиться на відстані 10 одиниць від центру сцени

        // Обертання сцени так, щоб ми бачили планети зверху (по осі X)
        //glRotatef(90.0f, 1.0f, 0.0f, 0.0f);  // Обертання на 90 градусів навколо осі X

        // Малюємо Сонце
        drawSun(0.0f, 0.0f, 0.0f, 1.0f);

        drawLine();
        // Анімація планети
        animatePlanet(earthAngle, 2.0f, 0.5f);  // Радіус орбіти = 2.0, швидкість обертання = 0.5
        drawRectangle(2.0f, -1.0f, -1.0f, 10.0f, 10.5f);
        glutSwapBuffers();  // Заміна буферів для подвійної буферизації

        glutMainLoopEvent();  // Обробка подій GLUT
    }

    return 0;
}
