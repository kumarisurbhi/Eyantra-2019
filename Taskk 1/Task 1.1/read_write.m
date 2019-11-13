global A = csvread('csv_matter.csv');  #do not change this line

################################################
#######Declare your global variables here#######
global accel_values = A(1:8000, 1:6);             #accelerometer values stored in different array  
global gyro_values  = A(1:8000, 7:12);            #gyro values stored 
axh = accel_values(:, 1);
axl = accel_values(:, 2);
ayh = accel_values(:, 3);
ayl = accel_values(:, 4);
azh = accel_values(:, 5);
azl = accel_values(:, 6);
gxh = gyro_values(:, 1);
gxl = gyro_values(:, 2);
gyh = gyro_values(:, 3);
gyl = gyro_values(:, 4);
gzh = gyro_values(:, 5);
gzl = gyro_values(:, 6); 
fcut = 5;
y1 = [];
y2 = [];
y3 = [];
z1 = [];
z2 = [];
z3 = [];
accel_sf = 16384;
gyro_sf = 131;
global B;
################################################


function read_accel(axl,axh,ayl,ayh,azl,azh)  
  
  #################################################
  ####### Write a code here to combine the ########
  #### HIGH and LOW values from ACCELEROMETER #####
  axl = dec2bin(axl, 8);
  axh = dec2bin(axh, 8);
  ax = strcat(axh, axl);  
  ayl = dec2bin(ayl, 8);
  ayh = dec2bin(ayh, 8);
  ay = strcat(ayh, ayl);  
  azl = dec2bin(azl, 8);
  azh = dec2bin(azh, 8);
  az = strcat(azh, azl); 
  ax = bin2dec(ax);
  ay = bin2dec(ay);
  az = bin2dec(az);
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
  gxl = dec2bin(gxl, 8);
  gxh = dec2bin(gxh, 8);
  gx = strcat(gxl, gxh);
  gyl = dec2bin(gyl, 8);
  gyh = dec2bin(gyh, 8);
  gy = strcat(gyl, gyh); 
  gzl = dec2bin(gzl, 8);
  gzh = dec2bin(gzh, 8);
  gz = strcat(gzl, gzh); 
  gx = bin2dec(gx);
  gy = bin2dec(gy);
  gz = bin2dec(gz);
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
   highpassfilter(ax,ay,az,f_cut);
  #####################################################;

endfunction



function lowpassfilter(ax,ay,az,f_cut)
  dT = 0.01;  #time in seconds
  Tau= 10;
  alpha = Tau/(Tau+dT);                #do not change this line
  
  ################################################
  ##############Write your code here##############
  y1(1, 1) = alpha* ax(1, : );
  y2(1, 1) = alpha* ay(1, : );
  y3(1, 1) = alpha* az(1, : );

  for n = 2:8000
     y1(n,1) = (1-alpha)*ax(n, :) + alpha*y1(n-1, 1);
     y2(n,1) = (1-alpha)*ay(n, :) + alpha*y2(n-1, 1);
     y3(n,1) = (1-alpha)*az(n, :) + alpha*y3(n-1, 1);
  endfor
  ################################################
  
endfunction



function highpassfilter(gx,gy,gz,f_cut)
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
  alpha = 0.03;
  ##############################################
  ####### Write a code here to calculate  ######
  ####### PITCH using complementry filter ######
    for i = 1:8000
     angleX(i,1) = (atan(ay(i,1)/ realsqrt(realpow(ax(i,1), 2) + realpow(az(i, 1), 2))))*(180/pi);
     angleY(i,1) = (atan( ax(i,1)/ realsqrt(realpow(ay(i,1), 2) + realpow(az(i,1), 2))))*(180/pi);
  endfor 
  ##############################################

endfunction 

function comp_filter_roll(ax,ay,az,gx,gy,gz)
  alpha = 0.03;
  ##############################################
  ####### Write a code here to calculate #######
  ####### ROLL using complementry filter #######
  
  ##############################################

endfunction 

function execute_code
global A;
global B;  

  for n = 1:rows(A)                    #do not change this line
    
    ###############################################
    ####### Write a code here to calculate  #######
    ####### PITCH using complementry filter #######
    ###############################################
    
  endfor
  csvwrite('output_data.csv',B);        #do not change this line
endfunction


execute_code                           #do not change this line
