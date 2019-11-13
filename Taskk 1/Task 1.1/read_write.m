global A = csvread('csv_matter.csv');  #do not change this line

################################################
#######Declare your global variables here#######

global y1 = [];
global y2 = [];
global y3 = [];
global z1 = [];
global z2 = [];
global z3 = [];

global pitch=0;
global roll=0;


global f_cut = 5;
global accel_sf = 16384;
global gyro_sf = 131;
################################################


function read_accel(axl,axh,ayl,ayh,azl,azh)  
  #################################################
  ####### Write a code here to combine the ########
  #### HIGH and LOW values from ACCELEROMETER #####
  global accel_sf;
  global f_cut;
  ax = bin2dec( strcat(dec2bin( axh , 8 ), dec2bin( axl , 8 )) );  
  ay = bin2dec( strcat(dec2bin( ayh , 8 ), dec2bin( ayl , 8 )) );  
  az = bin2dec( strcat(dec2bin( azh , 8 ), dec2bin( azl , 8 )) ); 
 
 for i = 1:8000
  
    if (ax(i,1)) > 32767
      ax(i,1) = (ax(i,1) - 65536);
    endif

    if (ay(i,1)) > 32767
      ay(i,1) = (ay(i,1) - 65536);
    endif
  
    if (az(i,1)) > 32767
      az(i,1) = (az(i,1) - 65536);
    endif
  
 endfor
  
  ax = ax/accel_sf;
  ay = ay/accel_sf;
  az = az/accel_sf;
 
 #################################################

  ####################################################
  # Call function lowpassfilter(ax,ay,az,f_cut) here #
  lowpassfilter(ax, ay,az, f_cut);
  ####################################################

endfunction

function read_gyro(gxl,gxh,gyl,gyh,gzl,gzh)
 
  #################################################
  ####### Write a code here to combine the ########
  ###### HIGH and LOW values from GYROSCOPE #######
  #################################################
  global gyro_sf;
  global f_cut;
  gx = bin2dec( strcat(dec2bin( gxh , 8 ), dec2bin( gxl , 8 )));  
  gy = bin2dec( strcat(dec2bin( gyh , 8 ), dec2bin( gyl , 8 )));  
  gz = bin2dec( strcat(dec2bin( gzh , 8 ), dec2bin( gzl , 8 ))); 
  
  for i = 1:8000
  
    if (gx(i,1)) > 32767
      gx(i,1) = (gx(i,1) - 65536);
    endif
  
    if (gy(i,1)) > 32767
      gy(i,1) = (gy(i,1) - 65536);
    endif
  
    if (gz(i,1)) > 32767
      gz(i,1) = (gz(i,1) - 65536);
    endif
  endfor

  gx = gx/gyro_sf;
  gy = gy/gyro_sf;
  gz = gz/gyro_sf;
  
  #####################################################
  # Call function highpassfilter(ax,ay,az,f_cut) here #
   highpassfilter(gx,gy,gz,f_cut);
  #####################################################;

 endfunction


function lowpassfilter(ax,ay,az,f_cut)
  global y1;
  global y2;
  global y3;
  dT = 0.01;                           #time in seconds
  Tau= 1/2*pi*f_cut;
  alpha = Tau/(Tau+dT);                #do not change this line
  
  ################################################
  ##############Write your code here##############
  
  y1(1, 1) = alpha* ax(1, : );
  y2(1, 1) = alpha* ay(1, : );
  y3(1, 1) = alpha* az(1, : );

  for n = 2:8000
     y1(n,1) = (1-alpha)*ax(n, :) + alpha*ax(n-1, 1);
     y2(n,1) = (1-alpha)*ay(n, :) + alpha*ay(n-1, 1);
     y3(n,1) = (1-alpha)*az(n, :) + alpha*az(n-1, 1);
  endfor
  ################################################
  
endfunction



function highpassfilter(gx,gy,gz,f_cut)
  global z1;
  global z2;
  global z3;
  dT = 0.01;  #time in seconds
  Tau= 1/2*pi*f_cut;
  alpha = Tau/(Tau+dT);                #do not change this line
  
  ################################################
  ##############Write your code here##############
  z1(1, 1) = gx(1, : );
  z2(1, 1) = gy(1, : );
  z3(1, 1) = gz(1, : );

  for n = 2:8000
     z1(n,1) = (1-alpha)*z1(n-1, 1) + (1 - alpha)*(gx(n , : ) - gx(n-1, : ));
     z2(n,1) = (1-alpha)*z2(n-1, 1) + (1 - alpha)*(gy(n , : ) - gy(n-1, : ));
     z3(n,1) = (1-alpha)*z3(n-1, 1) + (1 - alpha)*(gz(n , : ) - gz(n-1, : ));
  endfor
  ################################################
  
endfunction

function comp_filter_pitch(ax,ay,az,gx,gy,gz)
 global pitch;
  alpha1 = 0.03;
  gyroX=0;
  dT=0.01;
  ##############################################
  ####### Write a code here to calculate  ######
  ####### PITCH using complementry filter ######
#  for i = 1:8000
     angleX = (atan(ay/ realsqrt(realpow(ax,2) + realpow(az,2))))*(180/pi);
     gyroX =  gyroX + gx*dT;
     pitch = (1-alpha1)*gyroX + alpha1*angleX;

 # endfor 
  ##############################################

endfunction 

function comp_filter_roll(ax,ay,az,gx,gy,gz)
    global roll;
    alpha1 = 0.03;
    gyroY=0;
    dT=0.01;
  ##############################################
  ####### Write a code here to calculate #######
  ####### ROLL using complementry filter #######
  
       angleY = (atan(ax/ realsqrt(realpow(ay,2) + realpow(az, 2))))*(180/pi);
       gyroY = gyroY + gy*dT;
       roll = (1-alpha1)*gyroY + alpha1*angleY;
  ##############################################

endfunction 

function execute_code
  global y1;
  global y2;
  global y3;
   global z1;
  global z2;
  global z3;
  global A;
  global pitch;
  global roll;
  axh = A( : , 1  );
  axl = A( : , 2  );
  ayh = A( : , 3  );
  ayl = A( : , 4  );
  azh = A( : , 5  );
  azl = A( : , 6  ); 

  gxh = A( : , 7  );
  gxl = A( : , 8  );
  gyh = A( : , 9  );  
  gyl = A( : , 10 );
  gzh = A( : , 11 );
  gzl = A( : , 12 );
  
  read_accel(axl,axh,ayl,ayh,azl,azh);
  read_gyro(gxl,gxh,gyl,gyh,gzl,gzh);
  
  for n = 1:rows(A)                    #do not change this line
    
    ###############################################
    ####### Write a code here to calculate  #######
    ####### PITCH using complementry filter #######
    
    comp_filter_pitch(y1(n,1),y2(n,1),y3(n,1),z1(n,1),z2(n,1),z3(n,1));
    comp_filter_roll(y1(n,1),y2(n,1),y3(n,1),z1(n,1),z2(n,1),z3(n,1));
    B(n,1)=pitch;
    B(n,2)=roll;
    ###############################################
    
  endfor
  csvwrite('output_data.csv',B);        #do not change this line
endfunction


execute_code                           #do not change this line
