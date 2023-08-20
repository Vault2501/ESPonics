use <openscad-libraries/case.scad>


// wall thickness
wall                    = 2;
// gap for fitting
gap                     = 0.3;
// height of the lid
height_top              = 10;
// set to true to render the boards
show_boards             = false;
// Which object to render. Choices: top, bottom
object                  = "top";
// render boards (for debugging)
render_boards           = false;

// length of the case screws - determines case height
length_screw_case       = 35;
// diameter of the case screw holes
dia_screw_case          = 3.2+gap;
// length of the board screws
length_screw_board     = 8;
// diameter of screws for boards
dia_screw_board         = 2.2+gap;

// dimensions of boards
dim_boards             = [89.1,52.3,1.6];
// offset of boards in y direction
off_y_boards           = -2.6;
// x/y offset from the corner for screw holes of the boards
off_xy_screw_boards    = [5,8];

// x/z offset of the ethernet port
off_xz_port_ether       = [10,12];
// dist between ethernet ports
dist_port_ether         = 9.75;
// x/z dimension of the ethernet port
dim_port_ether          = [16+gap,wall+gap,13.5+gap];
// dimension of power switch
dim_switch_power        = [12,wall+gap,19];
// dimeter of power plug
dia_port_power         = 11.8;
// xz offset of power plug and switch
off_xz_power            = [10,10];


height_inside = length_screw_case-2*(gap+wall)+2*dia_screw_case;
length_inside = dim_boards[0];
width_inside  = dim_boards[1]+dim_boards[1]+wall+off_y_boards;

width_outside = width_inside+2*(2*dia_screw_case+wall+gap);

off_boards_xy = 2*dia_screw_case+wall+gap;
off_boards_z  = length_screw_board-dim_boards[2];

off_y_power   = width_outside-wall-gap;

echo(off_boards_xy);

if(render_boards)
{
    place_board_1() cube(dim_boards);
    place_board_2() cube(dim_boards);
}

difference()
{
    case(wall=wall,
         gap=gap,
         dia_screws=dia_screw_case,
         render=object,
         height_top=height_top,
         dim_inside=[length_inside,width_inside,height_inside]);
    ethernet_cutout();
    place_board_1(){
        board_screws_cutout();
    }
    place_board_2(){
        board_screws_cutout();
    }
    #power_connection();
}

if(object=="bottom" || object == "all")
{
    place_board_1(){
        difference()
        {
            board_screws_addon();
            #board_screws_cutout();
        }
    }

    place_board_2(){
        difference()
        {
            board_screws_addon();
            #board_screws_cutout();
        }
    }
}

module place_board_1()
{
    translate([off_boards_xy,off_boards_xy+off_y_boards,off_boards_z])
    {
        children();
    }
}

module place_board_2()
{
    translate([off_boards_xy,off_boards_xy+off_y_boards+dim_boards[1]+wall,off_boards_z])
    {
        children();
    }
}

module board_screws_cutout()
{
    place_screws(dim_boards[0],
                 dim_boards[1],
                 off_xy_screw_boards[0],
                 off_xy_screw_boards[1])
    {
        translate([0,0,-off_boards_z])
        {
            cylinder(d=dia_screw_board,h=length_screw_board-dim_boards[2]);
            cylinder(d=2*(dia_screw_board+gap),h=dia_screw_board+gap,$fn=6);
        }
    }
}

module board_screws_addon()
{
    place_screws(dim_boards[0],
                 dim_boards[1],
                 off_xy_screw_boards[0],
                 off_xy_screw_boards[1])
    {
        translate([0,0,-off_boards_z])
        {
            cylinder(d=dia_screw_board+wall,h=length_screw_board-dim_boards[2]);
            cylinder(d=2*(dia_screw_board)+wall,h=dia_screw_board+wall);
        }
    }
}

module ethernet_cutout()
{
    translate([off_boards_xy+off_xz_port_ether[0],
               0,
               off_boards_z+off_xz_port_ether[1]])
    {
        for(i=[0:dim_port_ether[0]+dist_port_ether:2*(dim_port_ether[0]+dist_port_ether)])
        {
            translate([i,0,0])
            {
                cube(dim_port_ether);
            }
        }
    }
}

module power_connection()
{
    translate([off_xz_power[0],off_y_power,off_xz_power[1]])
    {
        cube(dim_switch_power);
        translate([dim_switch_power[0]+20,0,dia_port_power/2])
        {
            rotate([270,0,0])
            {
                cylinder(d=dia_port_power,h=wall+gap);
            }
        }
    }
}