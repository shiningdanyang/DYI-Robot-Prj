void TB_ADC(int so_lan)
{
  int i=0,j=0,n=0;
  int max_cb[8]={0,0,0,0,0,0,0,0};
  int min_cb[8]={255,255,255,255,255,255,255,255};
  int MAX[8]   ={0,0,0,0,0,0,0,0};
  int KQ_MAX[8]={0,0,0,0,0,0,0,0};
  for(n=0;n<so_lan;n++)
  {
    for(j=0;j<100;j++)
    {
        doc_cam_bien();
        for(i=0;i<8;i++)
        {
/////////////////////////////// Lấy giá trị cb_max và cb_min ////////////////////////
            if(max_cb[0]<L[0])
                {max_cb[0]=L[0];}
            if(min_cb[0]>L[0])
                {min_cb[0]=L[0];}

            if(max_cb[1]<L[1])
                {max_cb[1]=L[1];}
            if(min_cb[1]>L[1])
                {min_cb[1]=L[1];}
            
            if(max_cb[2]<L[2])
                {max_cb[2]=L[2];}
            if(min_cb[2]>L[2])
                {min_cb[2]=L[2];}
            
            if(max_cb[3]<L[3])
                {max_cb[3]=L[3];}
            if(min_cb[3]>L[3])
                {min_cb[3]=L[3];}
            
            if(max_cb[4]<L[4])
                {max_cb[4]=L[4];}
            if(min_cb[4]>L[4])
                {min_cb[4]=L[4];}
            
            if(max_cb[5]<L[5])
                {max_cb[5]=L[5];}
            if(min_cb[5]>L[5])
                {min_cb[5]=L[5];}
            
            if(max_cb[6]<L[6])
                {max_cb[6]=L[6];}
            if(min_cb[6]>L[6])
                {min_cb[6]=L[6];}
            
            if(max_cb[7]<L[7])
                {max_cb[7]=L[7];}
            if(min_cb[7]>L[7])
                {min_cb[7]=L[7];}
/////////////////////////////////////////////////////////////////////////
        }

    }
/////////////Cộng dồn giá trị cb_max ////////////////////////////////////////
    MAX[0] = max_cb[0]+ MAX[0];
    MAX[1] = max_cb[1]+ MAX[1];
    MAX[2] = max_cb[2]+ MAX[2];
    MAX[3] = max_cb[3]+ MAX[3];
    MAX[4] = max_cb[4]+ MAX[4];
    MAX[5] = max_cb[5]+ MAX[5];
    MAX[6] = max_cb[6]+ MAX[6];
    MAX[7] = max_cb[7]+ MAX[7];
///////////////////////////////////////////////////////////////////////
  }
    for(i=0;i<n;i++)
    {
        KQ_MAX[i] = MAX[i]/n; // Lấy trung bình giá trị cb_max sau 800 lần lặp
        MAX[i]=0;
    }
    for(i=0;i<8;i++)
    {
        erom_ADC[i]=(KQ_MAX[i] + min_cb[i])/2;               //Approximation ADC
    }
    luu_CB(); // Luu ngưỡng giá trị cảm biên vào eeprom
}
