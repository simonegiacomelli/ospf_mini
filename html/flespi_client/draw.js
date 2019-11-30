
const DEFAULT_COLOR = 'grey';
let shapes = []
let shape_type = undefined;
let new_shape = false;

const RECTANGLE = 'rectangle';
const CIRCLE = 'circle';

class Shape{
    constructor(ctx, coords, color) {
        this.ctx = ctx;
        this.x = 0;
        this.y = 0;
        if (typeof coords !== 'undefined') {
            this.x = coords[0];
            this.y = coords[1];
        }
        this.color = ((typeof color !== 'undefined') ? color: DEFAULT_COLOR);
    }    
}

class Rectangle extends Shape{
    constructor(ctx, height, width, coords, color, degrees) {
        super(ctx, coords, color);   
        this.height = height;
        this.width = width;  
        this.degrees = ((typeof degrees !== 'undefined') ? degrees: 0);
    }

    paint() {
        this.ctx.save();
        this.ctx.translate(this.x + this.width/2, this.y+this.height/2);
        this.ctx.rotate(this.degrees * Math.PI / 180);
        this.ctx.translate(-(this.x + this.width/2), -(this.y+this.height/2));
        this.ctx.beginPath();        
        this.ctx.rect(this.x, this.y, this.width, this.height);
        this.ctx.fillStyle = this.color;
        this.ctx.fill();
        this.ctx.restore();
    }

    info(){
        console.log("(x,y)=(" + [this.x, this.y] + "), height="+ this.height + ", width="+this.width)
    }

    is_selected(x, y){
        return (y > this.y && y < this.y + this.height && x > this.x && x < this.x + this.height);
    }
}

class Circle extends Shape{
    constructor(ctx, coords, color, radius) {
        super(ctx, coords, color);    
        this.radius = radius; 
        this.color = ((typeof color !== 'undefined') ? color: DEFAULT_COLOR);
    }

    paint() {
        this.ctx.beginPath();
        this.ctx.arc(this.x, this.y, this.radius, 0, 2 * Math.PI, false);
        this.ctx.fillStyle = this.color;
        this.ctx.fill();
        this.ctx.restore();
    }

    is_selected(x, y){
        return Math.sqrt((x-this.x)*(x-this.x) + (y-this.y)*(y-this.y)) < this.radius;
    }
}


const create = (obj) => {
    shape_type = obj;
    new_shape = true;
}

const init = () => {
    let canvas = document.getElementById("myCanvas");
    let ctx = canvas.getContext("2d");

    const rect = canvas.getBoundingClientRect();
    const scaleX = canvas.width / rect.width;    // relationship bitmap vs. element for X
    const scaleY = canvas.height / rect.height;

    const width = canvas.width;
    const height = canvas.height;    

    // https://stackoverflow.com/questions/17130395/real-mouse-position-in-canvas
    let selected = undefined;

    const draw = () => {
        ctx.clearRect(0, 0, width, height);
        shapes.forEach((shape) => {
            shape.paint();
        });
    }

    const get_mouse_coord = (event) => {
        return [parseInt((event.clientX - rect.left) * scaleX), parseInt((event.clientY - rect.top) * scaleY)];
    } 

    // select/release an element
    canvas.addEventListener('mousedown', (event) => {
        if(new_shape == true){
            if(shape_type == RECTANGLE){
                shapes.push(new Rectangle(ctx, 20, 50, get_mouse_coord(event), 'red', 0))
            }
            if(shape_type == CIRCLE){
                shapes.push(new Circle(ctx, get_mouse_coord(event), 'green', 20));
            }
            shape_type == undefined;
            new_shape = false;
            draw();
            
        } else {
            if(selected == undefined){ 
                
                [x, y] = get_mouse_coord(event);
                
                shapes.forEach((shape, idx) => {
                    if (shape.is_selected(x, y)) {
                        console.log('object selected');
                        selected = idx;
                        // move selected shape to the end of the array (so it is drawn as last)
                        shapes.push(shapes.splice(selected, 1)[0]);
                        return;
                    }
                });   

            } else {
                selected = undefined;
                console.log("object released")
            }
        }
    });

    // move element
    canvas.addEventListener('mousemove', (event) => {
        // console.log("move");
        if(selected != undefined){
            console.log("dragging");
            [x, y] = get_mouse_coord(event);
            shapes[selected].x = x;
            shapes[selected].y = y;
            draw();
        }
    });
    
    draw();
}

