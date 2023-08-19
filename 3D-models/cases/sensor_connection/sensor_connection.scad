$fn=32;

render = "all";
ventilation = false;
connector = true;
wall = 2;
gap = 0.3;

len_board = 27;
wid_board = 26;

height_case = 20;

dia_GX16 = 6.3;
dia_sensor = 3.1;
dia_screw = 2.2+gap;

dia_screw_connector = 3.2+gap;

extra_GX16 = 20;
extra_sensor = 10;


len_case = len_board + extra_GX16 + extra_sensor + 2*wall;
wid_case = wid_board + 2*wall;

height_bottom_GX16=1.5*wall+dia_GX16/2;
height_bottom_sensor=1.5*wall+dia_sensor/2;


// render object
if(render == "bottom" || render == "all")
{
    bottom();
}

if( render == "top" || render == "all")
{
    translate([0,-wall,max(height_bottom_GX16,height_bottom_sensor)
                  +height_case
                  -max(height_bottom_GX16,height_bottom_sensor)])
    {
        rotate([180,0,0])
        {
            top();
        }
    }
}


// modules
module top()
{   
    difference()
    {
        case();
        case_cut_bottom(gap=gap);
    }
}
 
module bottom()
{   
    difference()
    {
        case();
        case_cut_top();
    }
    board_frame();
}

module case_cut_bottom(gap=0)
{
    if(connector==true)
    {
        connector();
    }
    // GX side
    cube([len_case/2-(abs(height_bottom_GX16-height_bottom_sensor)),
          wid_case,
          height_bottom_GX16]);
    
    difference()
    {
        translate([wall/2-gap,wall/2-gap,0])
        {
            cube([len_case/2-(abs(height_bottom_GX16-height_bottom_sensor))-wall/2+gap,
                  wid_case-wall+2*gap,
                  height_bottom_GX16+wall+gap]);
        }
        translate([wall/2-gap,wid_case/2-dia_GX16/2+gap,height_bottom_GX16])
        {
            cube([len_case/2-(abs(height_bottom_GX16-height_bottom_sensor)),
                 dia_GX16-2*gap,
                 wall+gap]);
        }
    }
    
    // transition
    hull ()
    {
        translate([len_case/2-(abs(height_bottom_GX16-height_bottom_sensor))-0.1,0,0])
        {
            cube([0.1,wid_case,height_bottom_GX16]);
            translate([2*(abs(height_bottom_GX16-height_bottom_sensor))+0.1,0,0])
            {
                cube([0.1,wid_case,height_bottom_sensor]);
            }
        }
    }
    hull ()
    {
        translate([len_case/2-(abs(height_bottom_GX16-height_bottom_sensor))-0.1,wall/2-gap,wall+gap])
        {
            cube([0.1,wid_case-wall+2*gap,height_bottom_GX16]);
            translate([2*(abs(height_bottom_GX16-height_bottom_sensor))+0.1,0,0])
            {
                cube([0.1,wid_case-wall+2*gap,height_bottom_sensor]);
            }
        }
    }
    
    
    // sensor side
    translate([len_case/2+(abs(height_bottom_GX16-height_bottom_sensor)),0,0])
    {
        cube([len_case/2-(abs(height_bottom_GX16-height_bottom_sensor)),
              wid_case,
              height_bottom_sensor]);
        difference()
        {
            translate([0,wall/2-gap,wall])
            {
                cube([len_case/2-(abs(height_bottom_GX16-height_bottom_sensor-wall/2))-wall+gap,
                      wid_case-wall+2*gap,
                      height_bottom_sensor+gap]);
            }
            translate([0,wid_case/2-dia_sensor/2+gap,height_bottom_sensor])
            {
                cube([len_case/2-(abs(height_bottom_GX16-height_bottom_sensor-wall/2))-wall+gap,
                      dia_sensor-2*gap,
                      wall+gap]);
            }
        }
    }
}

module case_cut_top()
{
    difference()
    {
        case_solid(mki=0);
        case_cut_bottom();
    }
}


module case()
{
    difference()
    {
        union()
        {
            case_solid(mki=6);
            if(connector==true)
            {
                connector();
            }
        }
        translate([wall,wall,wall])
        {
            cube_round([len_case-2*wall,
                        wid_case-2*wall,
                        height_case-2*wall],mki=2,plane="yz");
        }
        cutouts();
    }
    difference()
    {
        screw_holes(mode="additive");
        screw_holes(mode="subtractive");
    }
}

module case_solid(mki=4)
{
    cube_round([len_case,wid_case,height_case],mki=mki,plane="yz");
}

module board_frame()
{
    translate([wall+extra_GX16,wall,wall])
    {
        difference()
        {
            #cube([len_board+2*wall,wid_board,height_bottom_sensor]);
            translate([wall,0,0])
            {
                cube([len_board,wid_board,height_bottom_sensor]);
            }
            translate([0,(wid_case-3*dia_sensor)/2-wall,0])
            {
                cube([len_board+2*wall,3*dia_sensor,height_bottom_sensor]);
            }
        }
    }
}

module cutouts()
{
    hole_GX16();
    hole_sensor();
    screw_holes(mode="subtractive");
    if(ventilation==true)
    {
        ventilation_holes();
    }
}

module connector()
{
    len_connector=dia_screw_connector+3*wall;
    wid_connector=dia_screw_connector+4*wall;
    
    loc=[
         [-len_connector+wall,1.5*wall,0],
         [len_case-wall,1.5*wall,0],
        ];
    
    for(i=loc)
    {
        translate(i)
        {
            difference()
            {
                cube_round([len_connector,wid_connector,wall]);
                translate([dia_screw_connector/2+1.5*wall,dia_screw_connector/2+2*wall,0])
                {
                    cylinder(d=dia_screw_connector,h=wall);
                }
            }
        }
    }
}

module screw_holes(mode="additive")
{
    dia_screw_head = 2*dia_screw;
    loc_screws=[
                [wall+dia_screw/2,wall+dia_screw/2,0],
                [wall+dia_screw/2,wid_case-wall-dia_screw/2,0],
                [len_case-wall-dia_screw/2,wall+dia_screw/2,0],
                [len_case-wall-dia_screw/2,wid_case-wall-dia_screw/2,0]
               ];
    
    for(i=loc_screws)
    {
        translate(i)
        {
            if(mode=="subtractive")
            {
                cylinder(d=dia_screw,h=height_case);
                cylinder(d=dia_screw_head,h=dia_screw,$fn=6);
                translate([0,0,height_case-dia_screw])
                {
                    cylinder(d=dia_screw_head,h=dia_screw);
                }
            }
            if(mode=="additive")
            {
                cylinder(d=dia_screw+wall,h=height_case);
                cylinder(d=dia_screw_head+wall/2,h=dia_screw+wall,$fn=6);
                translate([0,0,height_case-dia_screw-wall])
                {
                    cylinder(d=dia_screw_head+wall/2,h=dia_screw+wall);
                }
            }
        }
    }
}

module hole_GX16()
{
    translate([0,wid_case/2,dia_GX16/2+1.5*wall])
    {
        rotate([0,90,0])
        {
            cylinder(h=wall,d=dia_GX16);
        }
    }
}

module hole_sensor()
{
    translate([len_case-wall,wid_case/2,dia_sensor/2+1.5*wall])
    {    
        rotate([0,90,0])
        {
            cylinder(h=wall,d=dia_sensor);
        }
    }
}


module ventilation_holes()
{
    intersection()
    {
        for(i=[2*wall : 2*wall : ((len_case-2*wall)/2*wall)])
        {
            translate([i,wall,height_case-wall])
            {
                cube([wall,wid_case-2*wall,wall]);
            }
        }
        for(i=[2*wall : 2*wall : ((wid_case-2*wall)/2*wall)])
        {
            translate([wall,i,height_case-wall])
            {
                cube([len_case-2*wall,wall,wall]);
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