$fn=32;

// print selection. Possible choices: bar,connector
print="bar";
// pipe diameter
pipe_dia=6;

// bar length
bar_length=150;
// bar width
bar_width=20;
// bar height
bar_height=6;
// bar bottom height
bar_bottom=2;
// connector_width
connector_width=10;
// space between connector cutouts
space=4.5;

connector_height=bar_height-bar_bottom;
connector_depth=bar_width-bar_width/4;
gap=0.2;

if(print=="bar")
{
    bar();
}
if(print=="connector")
{
    connector();
}
else
{
    echo("Unknown print selection");
}


module bar()
{
    difference()
    {        
        bar_shape();
        for(i=[space:space+connector_width:bar_length-space-connector_width])
        {
            translate([i,0,0])
            {
                #connector_shape();
            }
        }
    }
}

module connector()
{
    resize([connector_width-2*gap,connector_depth-2*gap,connector_height-2*gap])
    {
        connector_shape();
    }
    translate([connector_width-(connector_width/4)/2-2*gap,
               (pipe_dia+4)/2+(bar_width/4)/2,
               (pipe_dia+4)/2+bar_height-1.5])
    {
        rotate([270,0,90])
        {
            difference()
            {
                union(){    
                    translate([-(pipe_dia+4)/2,0,0])
                    {
                        cube([pipe_dia+4,(pipe_dia+4)/2,connector_width-connector_width/4-2*gap]);
                    }
                    cylinder(d=pipe_dia+4,h=connector_width-connector_width/4-2*gap);
                }
                cylinder(d=pipe_dia,h=connector_width-connector_width/4-2*gap);
                translate([-(pipe_dia+2)/2,-(pipe_dia+2)/2,0])
                {
                    rotate([0,0,-20])
                    {
                        #cube([pipe_dia+2,(pipe_dia+2)/2,connector_width-connector_width/4]);
                    }
                }
            }
        }
    }
}

module connector_shape()
{
    intersection()
    {
        translate([0,0,bar_bottom])
        {
            hull()
            {
                cube([connector_width,connector_depth,connector_height/2]);
                translate([(connector_width/4)/2,0,connector_height/2])
                {
                    cube([connector_width-connector_width/4,connector_depth,connector_height/2]);
                }
            }
        }
        bar_shape();
    }
}

module bar_shape()
{
    bottom=2;
    
    hull()
    {
        cube([bar_length,bar_width,bottom]);
        translate([0,(bar_width/4)/2,bottom])
        {
            cube([bar_length,bar_width-(bar_width/4),bar_height-bottom]);
        }
    }
}
