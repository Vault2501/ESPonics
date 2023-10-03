$fn=64;

dia_screw = 3.4;
wid_bar = 20;
thickness = 2;

extra = (wid_bar - dia_screw)/2;
wid_glue = 2*wid_bar;
len_glue = wid_glue;

bar_connector(dia_screw,wid_bar,thickness);

module bar_connector(dia,wid,thick)
{
    extra = (wid - dia)/2;
    wid_glue = wid;
    len_glue = 2 * wid_bar;
    
    difference()
    {
        cube([wid,wid,thick]);
        translate([extra+dia/2,extra+dia/2,0])
        {
            #cylinder(h = thick, d = dia);
        }
    }
    
    translate([0,wid_bar,0])
    {
        rotate([90,0,0])
        {
            difference()
            {
                cube([wid_glue,len_glue,thick]);
                translate([extra+dia/2,extra+dia/2,0])
                {
                    #cylinder(h = thick, d = dia);
                }
                translate([extra+dia/2,extra+dia/2+wid,0])
                {
                    #cylinder(h = thick, d = dia);
                }
            }
        }
    }
 
    stabilizer();
    translate([wid-thick,0,0])
    {
        stabilizer();
    }
    
    module stabilizer()
    {
        translate([0,0,wid+thick])
        {
            rotate([0,90,0])
            {
                linear_extrude(thick)
                {
                    polygon(points=[[wid,wid],[wid,0],[0,wid]], paths=[[0,1,2]]);
                }
            }
        }
    }
}
