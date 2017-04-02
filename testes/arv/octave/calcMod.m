% Retorna um vetor de tamanho n com 1 na posi��o (i+1) se h� algum j tal que
% v(j) mod n == i e 0 nas posi��es restantes.
function s = calcMod(v, n)
	% M�dulo
	%m = mod(v, n);
	m = v - n*floor(v ./ n); % Mais r�pido
	
	% Marca valores da imagem (resultados do m�dulo)
	A = zeros(1, n);
	A(m+1) = 1;
	s = A;
end