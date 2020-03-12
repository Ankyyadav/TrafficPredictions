
clear()
clc


//Function for finding distance
function distance = d(row,col)
    flag = 0
    
    for i=col+1:num_cells
        if pos_time(row,i) ~= N then
            distance = i - col
            flag = 1
            break
        end
    end
    
    if flag == 0 then
        for i=1:col-1
            if pos_time(row,i) ~= N then
                distance = num_cells - col + i
                break
            end
        end
    end

endfunction


//Function to get Yes or No for a given probability
function YesOrNo = probability(p)
    if  modulo(int(rand()*100000),100) < p*100 then
        YesOrNo = 1
    else
        YesOrNo = 0
    end
endfunction






//Initialisation
rho=0.3
L = 300
num_cells = int((L/7.5))

tot_time = 20
Vmax = 5
prob = 0.3
alpha = 0.4
beta0 = 0.4

Void=poly(0,'o')
N=-1
//No car position and zero car position will coincide so poly N for no car
pos_time=zeros(tot_time+1,num_cells) + N 
pos_time2 = zeros(tot_time+1,num_cells) + Void



//Ensuring that the next car is atleast d/2 distance away while filling initially
i=1
while i < num_cells
    if probability(rho) == 1 then
        pos_time(1,i)=grand(1,1,"uin",0,Vmax)
        i = i + (pos_time(1,i)+1)/2 +1
    else
        i=i+1
    end
end


//pos_time(1,:)=[N,N,N,N,N,N,N,1,N,N,5,N,N]
//pos_time(1,:)=[N,N,5,N,N,N,N,4,N,N,N,N,5,N,N,N,N,N,2,N,N,0,0,N,1,N,N,N,0,N,N,N,3,N,N,N,N,N,5,N]

for t = 1:tot_time+1
    for x = 1:num_cells 
        if pos_time(t,x) >=0 then
            pos_time2(t,x)=pos_time(t,x)
        end
    end
end




disp(pos_time2(1,:),"Initial positions")

disp("LOOP Bigins")
//Loop Begins
for t=2:tot_time+1
    
    
    for x=num_cells:-1:1


        if  pos_time(t-1,x) ~= N then
            flag2 = 0
            
//STEP 1            
            if pos_time(t-1,x) < Vmax && d(t-1,x) > pos_time(t-1,x) + 1 then
                pos_time(t,x) =  pos_time(t-1,x) + 1
                flag2 = 1
            end

//STEP 2
            if d(t-1,x) <= pos_time(t-1,x) && pos_time(t-1,x) ~=0 then 
                pos_time(t,x) = d(t-1,x) - 1
                flag2 = 1
            end
            
        
//STEP 3
            if probability(prob) == 1 && pos_time(t-1,x) ~=0 then
                pos_time(t,x) = pos_time(t-1,x) - 1
                flag2 = 1
            end

//If none of the steps were taken            
            if flag2 == 0 then
                pos_time(t,x)=pos_time(t-1,x)
            end
            

//STEP 4
            if x+pos_time(t,x) > num_cells then 
                    pos_time(t,x +pos_time(t,x) - num_cells) = pos_time(t,x)
            
            else
                    pos_time(t,x + pos_time(t,x)) = pos_time(t,x)
            end
             
            if pos_time(t,x) ~= 0
                    pos_time(t,x) = N
            end
            
        end //end of checking cars
        
    end //end or position loop
//Display            
    //disp(pos_time(t,:))
end //end of time loop


for t = 1:tot_time+1
    for x = 1:num_cells 
        if pos_time(t,x) >=0 then
            pos_time2(t,x)=pos_time(t,x)
            plot2d(x,t)
            a=gca()
            a.children(1).children.line_mode="off"
            a.children(1).children.mark_mode="on"
            a.children(1).children.mark_style= 0 //pos_time(t,x)
            a.children(1).children.mark_size= 1//pos_time(t,x)*3 +1
            a.grid_style = [7,7]
            a.x_location = "top"
            a.axes_reverse = ["off","on"]
            a.data_bounds=[0,0;num_cells+1,tot_time+2]
        end
    end
end
xgrid(1)

disp(pos_time2,"Positions after each time step")










