$fn=64;

filter_diameter=60;
filter_height=10;
outlet_diameter=9.6;
wall=2.4;
hole_num=8;
gap=0.2;

render="bottom";


module hole_ring(num=12,diameter=100,height=10)
{    
    for (i = [0:0.5:num]) {
        translate([sin(360*i/(num/2))*diameter/2, cos(360*i/(num/2))*diameter/2, 0 ])
        cylinder(h = height, r=3.14*diameter/num/4);
    }   
}

module holes_cutout()
{
    for (i = [1:0.5:2.5]) {
        echo(i);
        cutout_diameter=filter_diameter-(3.14*filter_diameter/hole_num/2)-wall;
        hole_ring(num=hole_num,
                  diameter=cutout_diameter-((i-1)*cutout_diameter/hole_num+((i-1)*cutout_diameter/i)),
                  height=wall);
    }
}


if (render == "bottom")
{
    difference()
    {
        cylinder(d=filter_diameter,h=filter_height-2*wall);
        translate([0,0,wall])
        {
            cylinder(d=filter_diameter-2*wall,h=filter_height-3*wall);
        }
        translate([0,0,filter_height-3*wall])
        {
            cylinder(d=filter_diameter-wall,h=wall);
        }
        holes_cutout();
        
    }
}

if (render == "top")
{
    difference()
    {
        union()
        {
            cylinder(d=filter_diameter-wall-2*gap,h=2*wall);
            translate([0,0,wall])
            {
                cylinder(d=filter_diameter,h=wall);
            }
            translate([filter_diameter/2-outlet_diameter/2-wall,0,2*wall])
            {
                difference()
                {
                    union()
                    {
                        cylinder(d=outlet_diameter+2*wall,h=20);
                        translate([0,0,20])
                        {
                            cylinder(d=outlet_diameter+4*wall,h=wall);
                        }
                    }
                    cylinder(d=outlet_diameter,h=20+wall);
                }
            }
        }
        cylinder(d=filter_diameter-2*wall,h=wall);
        translate([filter_diameter/2-outlet_diameter/2-wall,0,wall])
        {
            cylinder(d=outlet_diameter,h=wall);
        }
    }
}

if (render == "clip")
{
    difference()
    {
        union(){
            translate([-(outlet_diameter+3*wall),0,10/2])
            {
                hull()
                {
                    cube([outlet_diameter+4*wall,
                           (outlet_diameter+4*wall)/2,
                           10],center=true);
                    translate([-(outlet_diameter+4*wall)/2-wall/2,0,0])
                    {
                        cube([wall,
                              (outlet_diameter+4*wall),
                              10],center=true);
                    }
                }
            }
            cylinder(d=outlet_diameter+4*wall,h=10);
        }
        cylinder(d=outlet_diameter+2*wall+2*gap,h=10);
        translate([(outlet_diameter+4*wall)/1.5,0,10/2])
        {
            cube([outlet_diameter+4*wall,
                   outlet_diameter+4*wall,
                   10],center=true);
        }
    }
}
