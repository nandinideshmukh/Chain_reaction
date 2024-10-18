#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
using namespace std;
using namespace sf;

#define WIDTH 480
#define HEIGHT 1000

#define GRID_WIDTH 6
#define GRID_HEIGHT 13

#define FONT "fonts/Roboto/Roboto-Regular.ttf"

struct Cell
{
    int num_particles;
    Color color;
};

class ParticleGrid{
    int width;
    int height;
    Cell* particles;

    public:
    Cell& get(int x,int y){
        return particles[y*width+x];
    }

    int getWidth() { return width; }
    int getHeight() { return height; }

    ~ParticleGrid()
	{
	    delete[] particles;
	}

    ParticleGrid(int _w = GRID_WIDTH,int _h = GRID_HEIGHT):width(_w),height(_h)
    {
        particles = new Cell[width * height];

        for(int i=0;i<width;i++){
            for(int j=0;j<height;j++){
                this->get(i,j).num_particles = 0;
                this->get(i,j).color = Color::Black;
            }
        }
    }

    void add(int x,int y,Color color){

        bool is_on_top_edge = (y == 0);
	    bool is_on_bottom_edge = (y == height-1);
	    bool is_on_left_edge = (x == 0);
	    bool is_on_right_edge = (x == width-1);

        int max_particles = 3;

        if(is_on_bottom_edge || is_on_top_edge){
            max_particles -= 1;
        }
        
        else max_particles -= 0;

        if(is_on_left_edge || is_on_right_edge){
            max_particles -= 1;
        }

        else max_particles -= 0;

        if((this->get(x,y).num_particles+1)>max_particles){
            cout<<"\nExploding ";
            this->get(x,y).num_particles = 0;
            
        if (!is_on_top_edge)
		{
		    this->add(x, y-1, color);
		}

		if (!is_on_bottom_edge)
		{
		    this->add(x, y+1, color);
		}

		if (!is_on_left_edge)
		{
		    this->add(x-1, y, color);
		}

		if (!is_on_right_edge)
		{
		    this->add(x+1, y, color);
		}

        }
        else{
            cout<<"\nNormal";
            this->get(x,y).num_particles++;
            this->get(x,y).color = color;
        }
    }
    int countGreencells(){
        int count = 0;
        for(int i=0;i<width;i++){
            for(int j=0;j<height;j++){
                if(particles[j*width+i].color == Color::Green && particles[j*width+i].num_particles>0)
                count++;
            }
        }
        return count;
    }
    int countRedcells(){
        int count = 0;
        for(int i=0;i<width;i++){
            for(int j=0;j<height;j++){
                if(particles[j*width+i].color == Color::Red && particles[j*width+i].num_particles>0)
                count++;
            }
        }
       return count;
    }
};

class VisualGrid{

    Vector2i padding;
    Vector2i size;

    //Cell size like particular cells
    Vector2f cell_size;

    // Screen size
    Vector2i screen_size;

    // Stroke width for applying thickness to shape
    float stroke_width;
    
    //Color of grid
    Color color;

    // Storing lines vertical and horizontal of the grid
    vector<RectangleShape> lines;

    public:

    //Constructor and grid is initialized in it
    VisualGrid(Vector2i _size    =  Vector2i(GRID_WIDTH, GRID_HEIGHT),
	Vector2i _screen_size       =  Vector2i(WIDTH, HEIGHT),
	Vector2i _padding           =  Vector2i(10, 10),
	float _stroke_width         =  9,
	Color _color                =  Color::Red)
	
	: size(_size),
	  screen_size(_screen_size),
	  padding(_padding),
	  stroke_width(_stroke_width),
	  color(_color)
    {
    float x_inc = (int)(screen_size.x - 2*padding.x) / (float)size.x;
	float y_inc = (int)(screen_size.y - 2*padding.y) / (float)size.y;

	cell_size.x = x_inc;
	cell_size.y = y_inc;

    //Vertical lines
	for (int i = 0; i <= _size.x; i++)
	{
	    RectangleShape line(Vector2f(stroke_width, screen_size.y - 2*padding.y));
	    line.move(Vector2f(padding.x + x_inc * i, padding.y));
	    lines.push_back(line);
	}

    // Horizontal lines
	for (int i = 0; i <= _size.y; i++)
	{
	    RectangleShape line(Vector2f(screen_size.x - 2*padding.x, stroke_width));
	    line.move(Vector2f(padding.x, padding.y + y_inc * i));
	    lines.push_back(line);
	}

    }

    // Rendering particles in the cell like displaying the particles of the cell
    void render(RenderWindow& win,ParticleGrid& particles,Font& f){

        for(RectangleShape& line:lines){
            line.setFillColor(color);
            win.draw(line);
        }

        Text text;
        text.setFont(f);
        
        text.setCharacterSize(30);

        for(int i=0;i<particles.getWidth();i++){
            for(int j=0;j<particles.getHeight();j++){
                Cell& cell = particles.get(i,j);

                if(cell.num_particles ==0) continue;

                text.setFillColor(cell.color);
                text.setString(to_string(cell.num_particles));
                
                text.setPosition(Vector2f(padding.x + cell_size.x*i+cell_size.x /2.5f,padding.y + cell_size.y * j + cell_size.y/4.0f));
		        win.draw(text);
            }
        }
    }
     Vector2i getGridCoord(Vector2i pos)
    {
	int x = floor((pos.x - padding.x) / cell_size.x);
	int y = floor((pos.y - padding.y) / cell_size.y);

	return Vector2i(x, y);
    }

    void setColor(Color c)
    {
	color = c;
    }
};

Color curr_color = Color::Red;
VisualGrid g;
ParticleGrid g1;

void mouse_clicked(Vector2i pos){
    Vector2i grid_coord = g.getGridCoord(pos);

    if(g1.get(grid_coord.x ,grid_coord.y).color!=curr_color && g1.get(grid_coord.x ,grid_coord.y).num_particles>0)
    return;

    g1.add(grid_coord.x , grid_coord.y , curr_color);

    if(curr_color == Color::Green){
        g.setColor(Color::Red);
        curr_color = Color::Red;
    }
    else{
        g.setColor(Color::Green);
        curr_color = Color::Green;
    }
}

void check_events(RenderWindow& win,bool& mouse){
    Event ev;
    Text text;
    Font font;

    text.setFont(font);
    text.setCharacterSize(50);
    if (!font.loadFromFile(FONT)) {
        cerr << "Error loading font" << endl;
        return;
        }
    while(win.pollEvent(ev)){
        if(ev.type == Event::Closed)
        win.close();
    }
    if(ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left && !mouse) 
    {
        mouse = true;
        mouse_clicked(Mouse::getPosition(win));
    
    }
    if (ev.type == Event::MouseButtonReleased && ev.mouseButton.button == Mouse::Left && mouse)
	{
	    mouse = false;
	}
}

void check_winner(RenderWindow& win,Font& font,bool& over){
    Text text;
    text.setFont(font);
    text.setCharacterSize(40);

    if (g1.countRedcells() == 0 && g1.countGreencells()==0){
        text.setString("Game starts!!");
        text.setFillColor(Color::Green);
        text.setPosition(Vector2f(WIDTH / 2 - 75, HEIGHT / 2 - 50));
        win.draw(text);
    }

    else if (g1.countRedcells() == 0) {
        if(g1.countGreencells()==1) text.setString("Red player chance!");
        else{
        text.setString("Green player wins!!");}
        text.setFillColor(Color::Green);
        text.setPosition(Vector2f(WIDTH / 2 - 75, HEIGHT / 2 - 50));
        win.draw(text);
        over = true;
    } 
    else if (g1.countGreencells() == 0) {
        if(g1.countRedcells()==1) text.setString("Green player chance!");
        else{
        text.setString("Red player wins!! ");}
        text.setFillColor(Color::Red);
        text.setPosition(Vector2f(WIDTH / 2 - 75, HEIGHT / 2 - 50));
        win.draw(text);
        over = true;
    }
}

int main(){
    
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Chain Reaction game started");
    bool mouse = false;
    bool over = false;
    Font font;
    Text text;
    text.setCharacterSize(30);

    if (!font.loadFromFile(FONT))
    {
        cerr << "Error loading font" << endl;
    }

    while (window.isOpen())
    {   
        check_events(window,mouse);

        window.clear();

        g.render(window,g1,font);
        check_winner(window,font,over);
        window.display();
    }

    return 0;
}


