# p r a g m a   o n c e  
  
 # i n c l u d e   " i o D u m m y C h a r . h "  
  
 c l a s s   i o P l a y S t a g e ;  
 c l a s s   i o P l a y M o d e ;  
 c l a s s   i o I N I L o a d e r ;  
 c l a s s   S P 2 P a c k e t ;  
  
 c l a s s   i o A t t a c h W e a p o n D u m m y   :   p u b l i c   i o D u m m y C h a r  
 { 	 	  
 p u b l i c :  
 	 e n u m   A t t a c h S t a t e  
 	 {  
 	 	 A S _ S T A R T ,  
 	 	 A S _ F I R E ,  
 	 	 A S _ W A I T ,  
 	 } ;  
  
 	 e n u m   W e a p o n T y p e  
 	 {  
 	 	 N o r m a l _ W e a p o n ,  
 	 	 A r e a _ W e a p o n  
 	 } ;  
  
 	 s t r u c t   C r e a t e I n f o  
 	 {  
 	 	 W e a p o n T y p e   w T y p e ;  
 	 	 D W O R D   d w I n d e x ;  
 	 } ;  
  
 p r o t e c t e d : 	 	  
 	 t y p e d e f   s t d : : v e c t o r < C r e a t e I n f o >   C r e a t e W e a p o n L i s t ;  
 	  
 	 A t t a c h S t a t e   m _ A t t a c h S t a t e ;  
 	 / / 4�0�  x�q���  ��8� 
 	 i n t   m _ n C r e a t e W e a p o n I n d e x ;  
 	 / / A r e a 4�0�  tǄ� 
 	 i o H a s h S t r i n g   m _ s z A r e a W e a p o n N a m e ;  
 	 / / 4�0�  $��K� 
 	 D 3 D X V E C T O R 3   m _ v C r e a t e W e a p o n O f f s e t ;  
 	 / / ��1�\�  4�0�  x�q���  ��8� 
 	 C r e a t e W e a p o n L i s t   m _ C r e a t e W e a p o n L i s t ;  
  
 	 f l o a t   m _ f V i s i b l e R a n g e ;  
  
 	 b o o l   m _ b A l w a y s V i s i b l e ;  
  
 	 / / �� �    y o n g   j u n   e f f e c t   �� � 
 	 i o H a s h S t r i n g   m _ s z A t t a c h E f f e c t ;  
 	 b o o l 	 	   m _ b N o t U s e T r a n s l a t e B y C o n v e y e r ;  
  
 p u b l i c :  
 	 v i r t u a l   v o i d   L o a d P r o p e r t y (   i o I N I L o a d e r   & r k L o a d e r   ) ;  
 	 v i r t u a l   v o i d   T r a n s l a t e B y C o n v e y e r (   c o n s t   D 3 D X V E C T O R 3   & v M o v e   ) ;  
 p r o t e c t e d :  
 	 v i r t u a l   v o i d   P r o c e s s S t a t e (   f l o a t   f T i m e r P e r S e c ,   D W O R D   d w P r e T i m e   ) ;  
  
 p u b l i c :  
 	 v i r t u a l   v o i d   S e t D i e S t a t e (   b o o l   b E f f e c t   ) ;  
 	  
 p r o t e c t e d :  
 	 / / p�t�0�  }�0� 
 	 v o i d   L o a d I n i t P r o p e r t y (   i o I N I L o a d e r   & r k L o a d e r   ) ;  
 	 v o i d   S e t F i r e S t a t e ( ) ;  
 	 v o i d   S e t W a i t S t a t e ( ) ;  
 	 / / 4�0�  ��1� 
 	 v o i d   C r e a t e W e a p o n ( ) ;  
 	 v o i d   C r e a t e A r e a W e a p o n ( ) ;  
 	  
 	 / / 4�0�  �p� 
 	 v o i d   R e m o v e W e a p o n ( ) ;  
 	 v o i d   C h e c k M o v e ( ) ;  
 	 v o i d   V i s i b l e C h e c k ( ) ;  
  
 	 / / �� �  y o n g   j u n  
 	 / / tǙӸ�  ��1� 
 	 v o i d   C r e a t e E f f e c t ( ) ;  
 	 v o i d   R e m o v e E f f e c t ( ) ;  
 	 / /  
 	  
 p u b l i c :  
 	 i o A t t a c h W e a p o n D u m m y (   i o E n t i t y G r o u p   * p G r p ,   i o P l a y M o d e   * p M o d e   ) ;  
 	 v i r t u a l   ~ i o A t t a c h W e a p o n D u m m y ( ) ;  
 } ;  
  
 i n l i n e   i o A t t a c h W e a p o n D u m m y *   T o A t t a c h W e a p o n D u m m y (   i o D u m m y C h a r   * p D u m m y   )  
 {  
 	 i f (   ! p D u m m y   | |   p D u m m y - > G e t T y p e ( )   ! =   D C T _ A T T A C H _ W E A P O N   )  
 	 	 r e t u r n   N U L L ;  
  
 	 r e t u r n   s t a t i c _ c a s t <   i o A t t a c h W e a p o n D u m m y *   > (   p D u m m y   ) ;  
 } 