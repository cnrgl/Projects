clear;
%%%%%%%%% particle swarm optimization / n dimension space %%%%%%%%%%%%%%
F = @(x) (x(1,:).^2+x(2,:).^2);  % non-given function definition or the unknown system equation 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
nD = 2; %space dimension 
ULx = [-10, 10; % Dim limits
        -10, 10];
S = 5; %Space number of one dim / uniform distribution
w=0.5; % avearaging weight
phiP = 0.3; % inertial best cognitive coefficient
phiG = 0.3; % total goal cognitive coefficient
conf = 0.3; % termination confidence

P=[]; % particle positions
V=[]; % particle velocities

for i=1:nD
  toS = (ULx(i,2)-ULx(i,1))/(S+1); %space precision
  xRange = linspace(ULx(i,1)+toS, ULx(i,2)-toS, S);
  xSpace = repmat(xRange,power(S,(i-1)),1); % sub-dims repeat
  xSpace = reshape(repmat(xSpace,1,power(S,(nD-i))),1,[]); %master dims repeat
  
  %same things for velocities / vels are also multidimensional vectors
  xVelRange = linspace(-abs(ULx(i,2)-ULx(i,1)), abs(ULx(i,2)-ULx(i,1)), S);
  xVel = repmat(xVelRange,power(S,(i-1)),1);
  xVel = reshape(repmat(xVel,1,power(S,(nD-i))),1,[]);
  
  P = [P; xSpace]; % initial positions
  V = [V; xVel];
endfor  


S*=S; % hereafter total number will be used
BP = P; %initialization best known position 
[valG G] = max(F(P),[],2);

loss = 1;
while (loss > conf)
  loss = 0;
  for i=1:S
    for n=1:nD
      rg = rand(1);
      rp = rand(1);
      V(n,i) = w*V(n,i) + phiP*rp*(BP(n,i)-P(n,i)) + phiG*rg*(P(n,G)-P(n,i));    
    endfor
    P(:,i) = P(:,i)+V(:,i);
    
    solBP = F(BP(:,i));
    solP = F(P(:,i));
    soll(1,i) = solP;
    loss = loss + solBP;  
    if (solP<solBP)
      BP(:,i) = P(:,i);
    endif
    if (solBP<valG)
      valG = solBP;
      G = i;
    endif
  endfor
  loss = tanh(loss/S)
  
  quiver(P(1,:),P(2,:),V(1,:),V(2,:));
  xlim([-10,10]);
  ylim([-10,10]);
 
  %b = waitforbuttonpress ();
  pause(0.1);
endwhile
