% Retorna um vetor de tamanho n com 1 na posição (i+1) se há algum j tal que
% v(j) mod n == i e 0 nas posições restantes.
function s = calcMod(v, n)
	% Módulo
	%m = mod(v, n);
	m = v - n*floor(v ./ n); % Mais rápido
	
	% Marca valores da imagem (resultados do módulo)
	A = zeros(1, n);
	A(m+1) = 1;
	s = A;
end