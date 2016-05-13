C ==========================================================================
      FUNCTION   UNIRNDHP ( Y )
C          needs the MATHLIB of the CERNlibraries
C ==========================================================================

      integer index
      real a(1000)
      data index /1000/
      save a,index

*      write (*,*) ' enter unirandhp',index,unirndhp,y
*     generates an equally distributed function between 0 and 1
      
      if (index.eq.1000) then
         call ranlux(a,1000)
         index = 0
      endif
      index = index+1
      unirndhp = a(index)
      if(unirndhp.le.0.or.unirndhp.ge.1)
     +          write(*,*) 'Alarm!!',index,unirndhp

*      write (*,*) ' exit unirandhp',index,unirndhp
      RETURN
      END
C ==========================================================================
      FUNCTION UNIRDNINIT(LUX, INT, K1, K2)
C ==========================================================================
C     0 <= LUX <= 4  luxury level
C     LUX > 24  the p=LUX
C     K1=K2=0 then init from INT (32bi)

      
      RETURN
      END
C ==========================================================================
      FUNCTION UNIRDNSAVE(LUX, INT, K1, K2)
C ==========================================================================
      CALL RLUXAT (LUX, INT, K1, K2)
      WRITE(*,*) ' save RANLUX ', LUX, INT, K1, K2
      RETURN
      END
C ==========================================================================
C ==========================================================================
      FUNCTION UNIRDNCONT(LUX, INT, K1, K2)
C ==========================================================================
      CALL RLUXGO (LUX, INT, K1, K2)
      WRITE(*,*) ' start RANLUX ', LUX, INT, K1, K2
      RETURN
      END
C ==========================================================================
