$fn=128;

object="cable_box_top";

dim_fan = [92.8,92.8,40];
wall_adapter = 4;
height_cylinder = 50;
dia_cylinder = 108;
dia_screw = 4.4;
distx_screw = 4.5;
disty_screw = distx_screw;
dim_cable_box = [60,20,dim_fan[2]+wall_adapter ];
wall_cable_box = 2;
dia_cable = 6.3;
dia_screw_cable_box = 3.5;


dim_adapter = dim_fan + [2*wall_adapter,2*wall_adapter,0];



if(object=="adapter")
{
    part_adapter();
}

if(object=="cable_box_top")
{
    cable_box(part="top");
}
    
module part_adapter()
{
    difference()
    {
        union()
        {
            adapter(dim_fan,wall_adapter,height_cylinder,dia_cylinder);
            cable_box(part="bottom");
        }
        screw_holes_adapter(dia_screw,distx_screw,disty_screw,wall_adapter,dim_adapter);
        screw_holes_cable_box(mode="subtractive");
        translate([dia_cable/2+2*wall_adapter,wall_adapter,dim_cable_box[2]/2])
        {
            rotate([90,0,0])
            {
                #cylinder(d=dia_cable,h=wall_adapter);
            }
        }
    } 
}

module screw_holes_adapter(dia,distx,disty,wall,dim)
{
    for(i = [[distx+wall,disty+wall,0],
             [distx+wall,dim[1]-(disty_screw+wall),0],
             [dim[0]-(distx+wall),dim[1]-(disty+wall),0],
             [dim[0]-(distx+wall),disty+wall,0]])
    {
        translate(i+[0,0,dim[2]])
        {
            cylinder(d=dia,h=wall);
        }
    }
}

module screw_holes_cable_box(mode="subtractive")
{
    dia_head = 2*dia_screw_cable_box+0.3;
    
    translate([0,wall_adapter,0])
    {
        rotate([90,0,0])
        {
            for(i = [
                     [wall_adapter+wall_cable_box+dia_head/2,wall_cable_box+dia_head/2,0],
                     [wall_adapter+wall_cable_box+dia_head/2,dim_cable_box[2]-(dia_head/2+wall_cable_box)-wall_adapter,0],
                     [dim_cable_box[0]-(dia_head/2+wall_cable_box),wall_cable_box+dia_head/2,0],
                     [dim_cable_box[0]-(dia_head/2+wall_cable_box),dim_cable_box[2]-(dia_head/2+wall_cable_box)-wall_adapter,0]
                    ])
            {
                if(mode=="subtractive")
                {
                    translate(i)
                    {
                        cylinder(d=dia_screw_cable_box,h=dim_cable_box[1]);
                        cylinder(d=dia_head,h=dia_screw_cable_box,$fn=6);
                        translate([0,0,dim_cable_box[1]-dia_screw_cable_box])
                        {
                            cylinder(d=dia_head,h=dia_screw_cable_box);
                        }
                    }
                }
                if(mode=="additive")
                {
                    translate(i)
                    {
                        #cylinder(d=dia_screw_cable_box+wall_cable_box,h=dim_cable_box[1]);
                        #cylinder(d=dia_head+2*wall_cable_box,h=dia_screw_cable_box+wall_cable_box);
                        translate([0,0,dim_cable_box[1]-dia_screw_cable_box-wall_cable_box])
                        {
                            #cylinder(d=dia_head+2*wall_cable_box,h=dia_screw_cable_box+wall_cable_box);
                        }
                    }
                }
            }
        }
    }
}


module cable_box(part="bottom")
{
    if(part=="bottom")
    {
        translate([0,-dim_cable_box[1]+wall_adapter,0])
        {
            difference()
            {
                union()
                {
                    cable_box_case();
                    translate([0,+dim_cable_box[1]-wall_adapter,0])
                    {
                        screw_holes_cable_box(mode="additive");
                    }
                    
                }
                cutoff(gap=0.3);
                
            }
        }
    }
    
    if(part=="top")
    {
        translate([0,-dim_cable_box[1]+wall_adapter,0])
        {
            difference()
            {
                union()
                {
                    cable_box_case();
                    translate([0,+dim_cable_box[1]-wall_adapter,0])
                    {
                        screw_holes_cable_box(mode="additive");
                    }
                }
                difference()
                {
                    cube(dim_cable_box);
                    cutoff(gap=0);
                }
                translate([0,+dim_cable_box[1]-wall_adapter,0])
                {
                    screw_holes_cable_box(mode="subtractive");
                }
            }
        }
    }
    
    module cable_box_case()
    {
        difference()
        {
            union()
            {
                cube_round(dim_cable_box,plane="yz");
            }
            translate([wall_cable_box,wall_cable_box,wall_cable_box])
            {
                #cube_round(dim_cable_box
                           -[2*wall_cable_box,wall_cable_box+wall_adapter,2*wall_cable_box],plane="yz");
            }
            translate([dim_cable_box[0]-wall_cable_box,
                      -dia_cable/2+dim_cable_box[1]-wall_adapter,
                      dim_cable_box[2]/2])
            {
                rotate([0,90,0])
                {
                    #cylinder(d=dia_cable,h=wall_cable_box);
                }
            }
        }
    }
    
    module cutoff(gap=0)
    {
        cube([dim_cable_box[0],
               -dia_cable+dim_cable_box[1]-wall_adapter-2*wall_cable_box,
               dim_cable_box[2]]);
        translate([wall_cable_box/2-gap,0,wall_cable_box/2-gap])
        {
            cube([dim_cable_box[0]-wall_cable_box+2*gap,
                   -dia_cable+dim_cable_box[1]-wall_adapter+wall_cable_box+gap-2*wall_cable_box,
                   dim_cable_box[2]-wall_cable_box+2*gap]);
        }
    }
}

module adapter(dim_fan,wall,height_cylinder,dia_cylinder)
{
    
    dim_adapter = dim_fan + [2*wall,2*wall,wall];
    difference()
    {
        cube_round(dim_adapter,mki=6);
        translate([wall,wall,0])
        {
            cube_round(dim_fan,mki=3);
        }
        translate([dim_adapter[0]/2,dim_adapter[1]/2,dim_fan[2]])
        {
            cylinder(d=dim_fan[0],h=wall);
        }
    }
    
    height_transition=2*abs(dia_cylinder-dim_adapter[0]);
    translate([dim_adapter[0]/2,dim_adapter[1]/2,dim_adapter[2]])
    {
        translate([0,0,height_transition])
        {
            difference()
            {
                cylinder(d=dia_cylinder,h=height_cylinder);
                cylinder(d=dia_cylinder-2*wall,h=height_cylinder);
            }
        }
        difference()
        {
            cylinder(d1=dim_adapter[0],d2=dia_cylinder,h=height_transition);
            cylinder(d1=dim_fan[0],d2=dia_cylinder-2*wall,h=height_transition);
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

