
if exist('fct')
    [I_init] = imread(f_name);
else
    clear all
    close all
    [filename,PathName] = uigetfile('*.bmp','Select the file');
    [I_init] = imread([PathName filename]);
end
I_red=(I_init(:,:,1));

levels = 256;
pix = 0.28;
n_bandes=18;

h=zeros(256,1);
for i=1:size(I_red,1)
    for j=1:size(I_red,2)
        h(I_red(i,j)+1)=h(I_red(i,j)+1)+1;
    end 
end
ch=cumsum(h)/sum(sum(h));
cnt=1;
while ch(cnt)<0.98
    cnt=cnt+1;
end

I_red_mod=min(I_red,cnt);

I_red_size = max(size(I_red_mod));

Hist_H=sum(I_red_mod,2);
Hist_H_Bandes_Reshape=reshape(Hist_H,[round(I_red_size/n_bandes),n_bandes]);
Hist_H_Bandes=sum(Hist_H_Bandes_Reshape,1);
%Hist_Hist_H_Bandes_2=ceil(Hist_H_Bandes_2/max(Hist_H_Bandes_2)*64);
Hist_H_Bandes_par=ceil(Hist_H_Bandes/324/(324/n_bandes));
Taux_Bul_Href = 0.1;
a_H = 100/(1-Taux_Bul_Href);
b_H = 100-a_H;
Taux_Bul_Bref = 0.25;
a_B = 100/(1-Taux_Bul_Bref);
b_B = 100-a_B;
Taux_Bul_H = mean(Hist_H_Bandes_par(1:3))/255;
%Taux_Bul_H = round(Taux_Bul_H/2)*20;% variation de 20%
eq_H = a_H*Taux_Bul_H+b_H;
Taux_Bul_H = round(eq_H/10)*10;% variation de 10%
Taux_Bul_B = mean(Hist_H_Bandes_par(n_bandes-2:n_bandes))/255;
%Taux_Bul_B = round(Taux_Bul_B/1)*20;% variation de 20%
eq_B = a_B*Taux_Bul_B+b_B;
Taux_Bul_B = max(round(eq_B/10)*10,0);% variation de 10%

Hist_H_Bandes_tot=ceil(Hist_H_Bandes/324/(324/n_bandes)/cnt*64);

diff_Hist_H_Bandes_tot=diff(Hist_H_Bandes_tot);
diff_Hist_H_Bandes=diff(Hist_H_Bandes_par);

[A_Max_diff_Hist, B_Max_diff_Hist] = max(flip(diff_Hist_H_Bandes_tot));
B_Max_diff_Hist=n_bandes-B_Max_diff_Hist;
diff_Hist_temp=diff_Hist_H_Bandes_tot((B_Max_diff_Hist+1):end);
[A_Min_diff_Hist, B_Min_diff_Hist] = min(diff_Hist_temp);
if sum(diff_Hist_temp<0)==0
    B_Min_diff_Hist=max(size(diff_Hist_temp))+1;
end
y_rect=(B_Max_diff_Hist+1)*round(324/n_bandes);
%h_rect=max(324/n_bandes,(B_Min_diff_Hist-1)*round(324/n_bandes));
h_rect=(B_Min_diff_Hist-1)*round(324/n_bandes);
if h_rect==0
    y_rect = y_rect-324/n_bandes;
    h_rect = 324/n_bandes;
end
Min_hist=B_Max_diff_Hist+B_Min_diff_Hist;
    
Mean_Hist_H= mean(Hist_H_Bandes_tot);
Min_Hist_H= min(Hist_H_Bandes_tot);
Max_Hist_H= max(Hist_H_Bandes_tot);
Ecart_Hist_H = std(Hist_H_Bandes_tot);

Sum_Hist_H=sum(diff_Hist_H_Bandes_tot);

indice_1_emul=(Max_Hist_H-Min_Hist_H)/Mean_Hist_H;
indice_2_emul=(Mean_Hist_H-Min_Hist_H)/Mean_Hist_H;

disp_str = [filename,' - B_Max_diff_Hist: ',num2str(B_Max_diff_Hist),' B_Min_diff_Hist: ',num2str(B_Max_diff_Hist+B_Min_diff_Hist-1)];
disp(disp_str)

x = -400;
Tail_front = 12;
col='black';

[val_max, ind_max] = max(Hist_H_Bandes_tot);
[val_min, ind_min] = max(Hist_H_Bandes_tot);
Max_deriv=B_Max_diff_Hist/64+0.5;
Min_deriv=B_Min_diff_Hist/64+0.5;

Hist_Haut=Hist_H_Bandes_tot(:,1:B_Max_diff_Hist);
[A_Min_diff_Hist_tot, B_Min_diff_Hist_tot] = min(flip(diff_Hist_H_Bandes_tot));
B_Min_diff_Hist_tot=n_bandes-B_Min_diff_Hist_tot;
Hist_Bas=Hist_H_Bandes_tot(:,B_Min_diff_Hist_tot:end);
Max_Hist_Haut=max(Hist_Haut);
Min_Hist_Haut=min(Hist_Haut);
Mean_Hist_Haut=mean(Hist_Haut);
Max_Hist_Bas=max(Hist_Bas);
Min_Hist_Bas=min(Hist_Bas);
Mean_Hist_Bas=mean(Hist_Bas);
indice_emul1_Haut=(Max_Hist_Haut-Min_Hist_Haut)/Mean_Hist_Haut;
indice_emul2_Haut=(Mean_Hist_Haut-Min_Hist_Haut)/Mean_Hist_Haut;
indice_emul1_Bas=(Max_Hist_Bas-Min_Hist_Bas)/Mean_Hist_Bas;
indice_emul2_Bas=(Mean_Hist_Bas-Min_Hist_Bas)/Mean_Hist_Bas;

figure(1)
subplot(1,5,1:3); imshow(I_init, map);


filename2=filename; filename2(9)=' '; filename2(18)=' ';

disp_str = [filename,' - ind_max: ',num2str(ind_max),' Max_Hist_H: ',num2str(Max_Hist_H)];
disp(disp_str)
text(x,50, filename2 , 'FontSize',Tail_front,'Color',col)

if (Max_Hist_H<53)
    if (ind_max>=26)
        if(Sum_Hist_H/(Max_Hist_H-Min_Hist_H))>0.9
            %text(x,100,'Very Low Interface','FontSize',Tail_front,'Color',col)
            text(0,1040,'-------------------------------------','FontSize',30,'Color','green')    
            text(x,100,['Pos high (%) = '  num2str(-1)],'FontSize',Tail_front,'Color',col)
            text(x,150,['Pos low (%) = '  num2str(-1)],'FontSize',Tail_front,'Color',col)
            y_im = 9999;
            h_im = 0;
        else
            y_im = y_rect+h_rect;
            h_im = round(324/n_bandes); 
            %text(x,100,'Low Interface !','FontSize',Tail_front,'Color',col)
            rectangle('Position',[0,y_im,25,h_im],'FaceColor','green','EdgeColor','g','LineWidth',0.1)
            rectangle('Position',[1000,y_im,25,h_im],'FaceColor','green','EdgeColor','g','LineWidth',0.1)
        end
    elseif (ind_max<26)&&(ind_max>17)
        if (Sum_Hist_H/(Max_Hist_H-Min_Hist_H))>0.9
            y_im = 9999;
            h_im = 0;
            %text(x,100,'Very Low Interface','FontSize',Tail_front,'Color',col)
            text(0,1040,'-------------------------------------','FontSize',30,'Color','green')               
            text(x,100,['Pos high (%) = '  num2str(-1)],'FontSize',Tail_front,'Color',col)
            text(x,150,['Pos low (%) = '  num2str(-1)],'FontSize',Tail_front,'Color',col)
        else
            y_im = y_rect+h_rect;
            h_im = round(324/n_bandes); 
            rectangle('Position',[0,y_im,25,h_im],'FaceColor','green','EdgeColor','g','LineWidth',0.1)
            rectangle('Position',[1000,y_im,25,h_im],'FaceColor','green','EdgeColor','g','LineWidth',0.1)
        end
    elseif (ind_max<12)&&(ind_max>4)
            y_im = y_rect-round(324/n_bandes);
            h_im = round(324/n_bandes);     
            %text(x,100,'High Interface !','FontSize',Tail_front,'Color',col)
            rectangle('Position',[0,y_im,25,h_im],'FaceColor','green','EdgeColor','g','LineWidth',0.1)
            rectangle('Position',[1000,y_im,25,h_im],'FaceColor','green','EdgeColor','g','LineWidth',0.1)
    elseif (ind_max<=4)
        if abs((Sum_Hist_H/(Max_Hist_H-Min_Hist_H)))<0.4
            %text(x,100,'Very High Interface','FontSize',Tail_front,'Color',col)
            y_im = 9999;
            h_im = 0;
            text(0,-50,'-------------------------------------','FontSize',30,'Color','green') 
            text(x,100,['Pos high (%) = '  num2str(101)],'FontSize',Tail_front,'Color',col)
            text(x,150,['Pos low (%) = '  num2str(101)],'FontSize',Tail_front,'Color',col)
        else
            y_im = y_rect;
            h_im = round(324/n_bandes);           
            %text(x,100,'High Interface !','FontSize',Tail_front,'Color',col)
            rectangle('Position',[0,y_im,25,h_im],'FaceColor','green','EdgeColor','g','LineWidth',0.1)
            rectangle('Position',[1000,y_im,25,h_im],'FaceColor','green','EdgeColor','g','LineWidth',0.1)
        end
    else
        y_im = y_rect;
        h_im = h_rect;
        rectangle('Position',[0,y_im,25,h_im],'FaceColor','green','EdgeColor','g','LineWidth',0.1)
        rectangle('Position',[1000,y_im,25,h_im],'FaceColor','green','EdgeColor','g','LineWidth',0.1)

        
    end
else %(Max_Hist_H>=53)
    if (sum(Hist_H_Bandes_tot==64)>=3)&&(sum(Hist_H_Bandes_tot==64)<=20)
        if(indice_1_emul<0.1)
            %text(x,100,'Emulsion dense','FontSize',Tail_front,'Color',col)
            y_im = 9999;
            h_im = 0;
        else
            %text(x,100,'Emulsion dense','FontSize',Tail_front,'Color',col)
            y_im = y_rect;
            h_im = h_rect;
            rectangle('Position',[0,y_im,25,h_im],'FaceColor','green','EdgeColor','g','LineWidth',0.1)
            rectangle('Position',[1000,y_im,25,h_im],'FaceColor','green','EdgeColor','g','LineWidth',0.1)  
        end
    elseif (sum(Hist_H_Bandes_tot==64)>=21) 
        %text(x,100,'Emulsion dense','FontSize',Tail_front,'Color',col)        
        y_im = 9999;
        h_im = 0;
    else %(sum(Hist_H_Bandes_tot==64)<3)
        if(indice_1_emul<0.45)
            %text(x,100,'Emulsion faible','FontSize',Tail_front,'Color',col)
            y_im = 9999;
            h_im = 0;
        else %(indice_1_emul>=0.3)
            if(Sum_Hist_H/(Max_Hist_H-Min_Hist_H))>0.9&&(ind_max>=26)
                %text(x,100,'Very Low Interface','FontSize',Tail_front,'Color',col)
                y_im = 1025;
                h_im = 0;
                text(0,1040,'-------------------------------------','FontSize',30,'Color','green')            
                text(x,100,['Pos high (%) = '  num2str(-1)],'FontSize',Tail_front,'Color',col)
                text(x,150,['Pos low (%) = '  num2str(-1)],'FontSize',Tail_front,'Color',col)
            elseif (ind_max>=26)
                %text(x,100,'Low Interface !','FontSize',Tail_front,'Color',col)             
                y_im = y_rect+h_rect;
                h_im = round(324/n_bandes);
                rectangle('Position',[0,y_im,25,h_im],'FaceColor','green','EdgeColor','g','LineWidth',0.1)
                rectangle('Position',[1000,y_im,25,h_im],'FaceColor','green','EdgeColor','g','LineWidth',0.1)  
            else
                y_im = y_rect;
                h_im = h_rect;
                rectangle('Position',[0,y_im,25,h_im],'FaceColor','green','EdgeColor','g','LineWidth',0.1)
                rectangle('Position',[1000,y_im,25,h_im],'FaceColor','green','EdgeColor','g','LineWidth',0.1)
                 
            end
        end
    end
% else
%     if(indice_1_emul<0.3)
%         text(x,100,'Emulsion ','FontSize',Tail_front,'Color',col)
%     elseif(Sum_Hist_H/(Max_Hist_H-Min_Hist_H))>0.9&&(ind_max>=26)
%         text(x,100,'Very Low Interface & Emulsion','FontSize',Tail_front,'Color',col)
%         text(0,1040,'-------------------------------------','FontSize',30,'Color','red')            
%     else
%         rectangle('Position',[0,y_rect,25,h_rect],'FaceColor','red','EdgeColor','r','LineWidth',0.1)
%         rectangle('Position',[1000,y_rect,25,h_rect],'FaceColor','red','EdgeColor','r','LineWidth',0.1)
% 
%     end
end


%Position=round((size(I_red,1)-(y_im+h_im/2))*pix);
%Largeur=round(h_im*pix);
P_haut=round(((324-y_im)/324)*100);
P_bas=round(((324-(h_im+y_im))/324)*100);


if (P_haut<=0)
    P_haut=-1;
    P_bas=-1;
elseif (P_haut>=92)
    P_hau=101;
    P_bas=101;
    
else    
    text(x,100,['Pos high (%) = '  num2str(P_haut,'%.0f')],'FontSize',Tail_front,'Color',col)
    text(x,150,['Pos low (%) = '  num2str(P_bas,'%.0f')],'FontSize',Tail_front,'Color',col)
end
text(x,200,['Taux high (%) = '  num2str(Taux_Bul_H,'%.0f')],'FontSize',Tail_front,'Color',col)
text(x,250,['Taux low (%) = '  num2str(Taux_Bul_B,'%.0f')],'FontSize',Tail_front,'Color',col)
%text(-100,250,'%','FontSize',Tail_front,'Color',col)



subplot(1,5,4); plot(Hist_H_Bandes_tot,'*-')
grid on
a=axis; a(2)=n_bandes; axis(a); camroll (270);

subplot(1,5,5); plot(diff_Hist_H_Bandes_tot,'*-')
grid on
a=axis; a(2)=n_bandes; axis(a); camroll (270);

set(gcf,'WindowState','maximized');

