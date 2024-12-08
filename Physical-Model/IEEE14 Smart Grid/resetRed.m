% Reset script

serie = [27824000,25076000,11817820,6916420,15090180,8831580,...
    20484800,11988800,19805590,11591290,16845710,9859010,12028310, ...
    7039610,17472840,10226040,14912240,8727440,14706090,8606790,...
    18199790,10651490,13888000,8128000];

address = "Ieee14/";
% Strings

sourcesName = "Three-Phase Source";
sourcesNameInd = [1 2 3 6 8];

loadsName = "Three-Phase Series RLC Load";
loadsNameInd = [2 3 4 5 6 9 10 11 12 13 14];


% Sources
aux = address + sourcesName + sourcesNameInd(2);

set_param(aux, 'Pref', mat2str(serie(1)));

aux = address + sourcesName + sourcesNameInd(3);

set_param(aux, 'Pref', mat2str(serie(2)));

% Loads
j = 3;
for i = 1:length(loadsNameInd)
    aux = address + loadsName + loadsNameInd(i);
    set_param(aux, 'ActivePower', mat2str(serie(j)));
    set_param(aux, 'InductivePower', mat2str(serie(j+1)));
    j = j + 2;
end

B1_L5 = "B1_L5";
B1_L4 = "B1_L4";
B2_G1 = "B2_G1";
B2_L5 = "B2_L5";
B2_L4 = "B2_L4";
B2_G3 = "B2_G3";
B3_L4 = "B3_L4";
B6_L11 = "L1";
B6_L12 = "L2";
B6_L13 = "L3";
B6_L13_2 = "L4";
B6_L10 = "B6_L10";
B6_L14 = "B6_L14";
B8_L5 = "B8_L5";
B8_L14 = "B8_L14";
B8_L10 = "B8_L10";

% Initialize SH

aux = address + "SC_" + B1_L5;
set_param(aux, 'Value', mat2str(0));

aux = address + "SC_" + B1_L4;
set_param(aux, 'Value', mat2str(0));

aux = address + "SC_" + B2_G1 ;
set_param(aux, 'Value', mat2str(0));

aux = address + "SC_" + B2_L5 ;
set_param(aux, 'Value', mat2str(0));

aux = address + "SC_" + B2_L4 ;
set_param(aux, 'Value', mat2str(0));

aux = address + "SC_" + B2_G3 ;
set_param(aux, 'Value', mat2str(0));

aux = address + "SC_" + B3_L4 ;
set_param(aux, 'Value', mat2str(0));

aux = address + "SC_" + B6_L11 ;
set_param(aux, 'Value', mat2str(0));

aux = address + "SC_" + B6_L12 ;
set_param(aux, 'Value', mat2str(0));

aux = address + "SC_" + B6_L13;
set_param(aux, 'Value', mat2str(0));

aux = address + "SC_" + B6_L13_2;
set_param(aux, 'Value', mat2str(0));

aux = address + "SC_" + B6_L10 ;
set_param(aux, 'Value', mat2str(0));

aux = address + "SC_" + B6_L14 ;
set_param(aux, 'Value', mat2str(0));

aux = address + "SC_" + B8_L5 ;
set_param(aux, 'Value', mat2str(0));

aux = address + "SC_" + B8_L14;
set_param(aux, 'Value', mat2str(0));

aux = address + "SC_" + B8_L10;
set_param(aux, 'Value', mat2str(0));