%cle






%p:l;;l;22222

%2222222222


close all

Folder_list = dir('images*');
for iii = 1 : max(size(Folder_list))
    Folder_name = Folder_list(iii).name;
    cd (Folder_name);
    filelist = dir('O.bmp');
    filename = filelist(1).name;
    f_name = [Folder_name '\' filename];
    cd ..
    fct = 1;
    test_seule_images_3_bandes_2023_03_22
    clear fct
    cd new_images
    %saveas(gcf,[filename(1:18) '.bmp'])
    f=char(filename);
    %C=table({f},cnt,y_rect,y_rect+h_rect,ind_max,Min_hist,B_Max_diff_Hist,Mean_Hist_H,Min_Hist_H,Max_Hist_H,Ecart_Hist_H);
    %writetable(C,'valeurs.xlsx',"WriteMode","append","AutoFitWidth",false);
    C1=table({f},P_haut,P_bas,Taux_Bul_H,Taux_Bul_B);
    writetable(C1,'Mesures.xlsx',"WriteMode","append","AutoFitWidth",false);
    %C2=table({f},Max_Hist_Haut,Min_Hist_Haut,Mean_Hist_Haut,indice_emul1_Haut,Max_Hist_Bas,Min_Hist_Bas,Mean_Hist_Bas,indice_emul1_Bas);
    %writetable(C2,'Mesures_2.xlsx',"WriteMode","append","AutoFitWidth",false);
    %close all
    cd ..
end
clear fct
end