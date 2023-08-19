$fn=64;

// generic

// wall thickness
wall                    = 2;
// gap for fitting
gap                     = 0.3;
// height of the bottom part
height_bottom           = 30;
// set to true to render the boards
show_boards             = false;
// Which object to render. Choices: top, bottom
object                  = "bottom";

// case - most values are calculated

// length of the case screws - determines case height
length_screw_case       = 35;
// diameter of the case screw holes
dia_screw_case          = 3.2+gap;
// diameter of the head for the case screw holes
dia_screw_head_case     = 6.5+gap;
// additional space for the case in the y direction
extra_y_case            = 12;

// generic board values

// length of the board screws - determines the 
length_screw_boards     = 7.5;

// board 1 values and ports

// dimensions of board 1
dim_board_1             = [51,38.2,1.6];
// diameter of screws for board 1
dia_screw_board_1       = 2.2+gap;
// diameter of screw heads for board 1
dia_screw_head_board_1  = 4.4+gap;
// offset of board 1 in y direction
off_y_board_1           = 10;
// x/y offset from the corner for screw holes of board 1
off_xy_screw_board_1    = [3.2,3.2];
// x/z offset of the ethernet port in board 1
off_xz_port_ether       = [19,13];
// x/z dimension of the ethernet port
dim_port_ether          = [16+gap,wall,13.5+gap];

// board 2 values and ports

// dimensions of board 2
dim_board_2             = [72.2,52,1.6];
// diameter of screws for board 2
dia_screw_board_2       = 3.2+gap;
// diameter of screw heads for board 2
dia_screw_head_board_2  = 6.5+gap;
// offset of board 2 in y direction
off_y_board_2           = 15;
// x/y offset from the corner for screw holes of board 2
off_xy_screw_board_2    = [3.2,3.2];


// board independent ports

// diameter of the GX16 sockets
dia_port_GX16          = 16.2+gap;
// height for plugs
off_z_plugs            = 17;
// number of GX16 sockets
count_GX16             = 3;
// space between GX16 sockets
spacing_GX16           = 8;
// power plug
dia_port_power         = 12;
// power switch
dim_switch_power       = [12,19];
// antenna hole
dia_port_antenna       = 2;
// antenna location
off_xz_antenna         = [60,30];

// calculated values
off_x_plugs = dia_screw_head_case+2*wall+dia_port_GX16/2+dim_board_1[0]+3;
off_x_power = off_x_plugs+count_GX16*dia_port_GX16+count_GX16*spacing_GX16;
off_z_power = 15;
off_x_switch = dia_screw_head_case+2*wall;
off_z_switch = wall+6;

dim_case    = [2*(dia_screw_head_case+2*wall)+dim_board_1[0]+wall+dim_board_2[0]+dim_switch_power[0],
               wall+gap+gap+wall+extra_y_case+max((off_y_board_2+dim_board_2[1]),
                                                  (off_y_board_1+dim_board_1[1])),
               length_screw_case+dia_screw_head_case/2];
               
height_top  = dim_case[2]-height_bottom;
//height_front = off_z_plugs+dia_port_GX16/2;
height_base_boards = length_screw_boards - wall - dim_board_1[2];

// main object
if (object == "top")
{
    case_top();
}
if (object == "bottom")
{
    case_bottom();
}


// modules

// top empty case
module case_top()
{
    difference()
    {
        case();
        cube([dim_case[0],dim_case[1],height_bottom]);
        //#cube([dim_case[0],20,height_front]);
        connector(0);
    }
}

// bottom empty case
module case_bottom()
{
    difference()
    {
        case();
        translate([0,0,height_bottom])
        {
            cube([dim_case[0],dim_case[1],height_top]);
        }
    }
    connector(gap);
}

// complete case
module case()
{
    difference()
    {
        case_pos();
        case_neg();
    }
}

// items to add to the case
module case_pos()
{

    
    difference()
    {
        cube_round(dim_case,5);
        translate([wall,wall,wall])
        {
            cube_round([dim_case[0]-2*wall,
                        dim_case[1]-2*wall,
                        dim_case[2]-2*wall],3);
        }
    }
    
    screws(dia_screw_case,
           dia_screw_head_case,
           length_screw_case,
           [wall+dia_screw_head_case/2,wall+dia_screw_head_case/2],
           [dim_case[0],dim_case[1],dim_case[2]-dia_screw_head_case/2],
           wall,
           mode="shell_bottom");
    
    screws(dia_screw_case,
           dia_screw_head_case,
           length_screw_case,
           [wall+dia_screw_head_case/2,wall+dia_screw_head_case/2],
           [dim_case[0],dim_case[1],dim_case[2]-dia_screw_head_case/2],
           wall,
           mode="shell_top");
    
    place_board_1()
    {
        screws(dia_screw_board_1,
               dia_screw_head_board_1,
               length_screw_boards,
               off_xy_screw_board_1,
               dim_board_1,
               wall,
               mode="shell_bottom");
        
        if(show_boards == true)
        {
            cube_round(dim_board_1,5);
        }
        port_ether(mode="block");
    }
    place_board_2()
    {
        screws(dia_screw_board_2,
               dia_screw_head_board_2,
               length_screw_boards,
               off_xy_screw_board_2,
               dim_board_2,
               wall,
               mode="shell_bottom");
        
        if(show_boards == true)
        {
            cube(dim_board_2,5);
        }
    }
}

// elements to be cut out of the case
module case_neg()
{
    screws(dia_screw_case,
           dia_screw_head_case,
           length_screw_case,
           [wall+dia_screw_head_case/2,wall+dia_screw_head_case/2],
           [dim_case[0],dim_case[1],dim_case[2]-dia_screw_head_case/2],
           wall,
           mode="cutout");
    
    port_plugs(dia=dia_port_GX16,
                off_x=off_x_plugs,
                off_z=off_z_plugs,
                spacing=spacing_GX16,
                count=count_GX16,
                wall=wall);
    
    place_board_1()
    {
        screws(dia_screw_board_1,
               dia_screw_head_board_1,
               length_screw_boards,
               off_xy_screw_board_1,
               dim_board_1,
               wall,
               mode="cutout");
        port_ether(mode="cutout");
    }
    place_board_2()
    {
        screws(dia_screw_board_2,
               dia_screw_head_board_2,
               length_screw_boards,
               off_xy_screw_board_2,
               dim_board_2,
               wall,
               mode="cutout");
    }
    // power, antenna and switch
    translate([off_x_power,0,off_z_power])
    {
        rotate([270,0,0])
        {
            #cylinder(d=dia_port_power,h=wall);
        }
    }
    translate([off_x_switch,0,off_z_switch])
    {
        #cube([dim_switch_power[0],wall,dim_switch_power[1]]);
    }
    translate([off_xz_antenna[0],0,off_xz_antenna[1]])
    {
        rotate([270,0,0])
        {
            #cylinder(d=dia_port_antenna,h=wall);
        }
    }
}

// place modules
module place_board_1() 
{
    translate([dia_screw_head_case+2*wall,
               wall+gap+off_y_board_1,
               wall+height_base_boards])
    {
        children();
    }
}

module place_board_2() 
{
    translate([dia_screw_head_case+2*wall+dim_board_1[0]+wall,
               wall+gap+off_y_board_2,
               wall+height_base_boards])
    {
        children();
    }
}

// port cutouts
module port_ether(mode)
{
    if(mode == "cutout")
    {
        translate([off_xz_port_ether[0],
                   -off_y_board_1-gap-wall,
                   off_xz_port_ether[1]])
        {
            cube(dim_port_ether);
        }
    }
    if(mode == "block")
    {
        translate([off_xz_port_ether[0],
                   -off_y_board_1-gap,
                   off_xz_port_ether[1]+dim_port_ether[2]])
        {
            cube([dim_port_ether[0],5,dim_case[2]-2*wall-height_base_boards-(off_xz_port_ether[1]+dim_port_ether[2])]);
        }
    }
} 


module port_plugs(dia,off_x,off_z,spacing,count,wall)
{
    for(i=[off_x:dia+spacing:(count*(dia+spacing)+off_x)-1])
    {
        translate([i,0,off_z])
        {
            rotate([270,0,0])
            {
                cylinder(d=dia,h=wall);
            }
        }
    }
}

// small overhang to align top and bottom
module connector(gap)
{
    translate([wall/2+gap,dim_case[1]/4+gap,0])
    {
        cube([wall-gap,dim_case[1]/2-2*gap,height_bottom+2*wall-gap]);
    }
    translate([dim_case[0]-wall-wall/2,dim_case[1]/4+gap,0])
    {
        cube([wall-gap,dim_case[1]/2-2*gap,height_bottom+2*wall-gap]);
    }
}

// screw module
module screws(dia,
              dia_head,
              length,
              off_xy,
              dim,
              wall,
              mode="screw")
{
    for(i=[[off_xy[0],off_xy[1]],
           [dim[0]-off_xy[0],off_xy[1]],
           [dim[0]-off_xy[0],dim[1]-off_xy[1]],
           [off_xy[0],dim[1]-off_xy[1]]])
    {
        echo(i);
        translate([i[0],i[1],-length+dim[2]])
        {
            if (mode=="screw")
            {
                cylinder(d=dia,h=length);
                translate([0,0,length])
                {
                    cylinder(d=dia_head,h=dia_head/2);
                }
            }
            if (mode=="cutout")
            {
                cylinder(d=dia,h=length);
                cylinder(d=dia_head,h=dia_head/2,$fn=6);
                translate([0,0,length])
                {
                    cylinder(d=dia_head,h=dia_head);
                }
            }
            if (mode=="shell_bottom")
            {
                hull()
                {
                    cylinder(d=dia+2*wall,h=length-dim[2]);
                    cylinder(d=dia_head+2*wall,h=dia_head/2+wall);
                }
            }
            if (mode=="shell_top")
            {
                translate([0,0,length-wall])
                {
                    cylinder(d=dia_head+2*wall,h=dia_head/2+wall);
                }
                translate([0,0,height_bottom])
                {
                    cylinder(d=dia+2*wall,h=height_top);
                }
            }
            if (mode=="closure_bottom")
            {
                translate([0,0,dia_head/2])
                {
                    cylinder(d=dia_head+2*wall,h=0.2);
                }
            }
            if (mode=="closure_bottom")
            {
            translate([0,0,length])
                {
                    cylinder(d=dia_head+2*wall,h=0.2);
                }
            }
        }
        
    }
}

module cube_round(dim,mki=5,plane="xy"){
    if(mki<=0)
    {
        cube(dim);
    }
    else
    {
        if(plane=="xy")
        {
            translate([mki/2,mki/2,0])
            {
                linear_extrude(dim[2])
                {
                    minkowski()
                    {
                        square([dim[0]-mki,dim[1]-mki]);
                        circle(d=mki);
                    }
                }
            }
        }
        if(plane=="yz")
        {
            translate([0,mki/2,dim[2]-mki/2])
            {
                rotate([0,90,0])
                {
                    linear_extrude(dim[0])
                    {
                        minkowski()
                        {
                            square([dim[2]-mki,dim[1]-mki]);
                            circle(d=mki);
                        }
                    }
                }
            }
        }
        if(plane=="xz")
        {
            translate([mki/2,dim[1],mki/2])
            {
                rotate([90,0,0])
                {
                    linear_extrude(dim[1])
                    {
                        minkowski()
                        {
                            square([dim[0]-mki,dim[2]-mki]);
                            circle(d=mki);
                        }
                    }
                }
            }
        }
    }
}

