% Gera todas as máscaras Ou possíveis
function mascs = mascsOu(graus)
	
	% Nenhum ou um bit para cada nível (2 ou mais garantem colisão)
	mascs(1) = 0;
	soma = 0;
	for nivel = 1:length(graus)
		n = length(mascs);
		grau = graus(nivel);
		
		% mascs vira [mascs mascs ...] grau+1 vezes
		for i = 1:grau
			mascs = [mascs mascs(1:n)];
		end
		
		% Cada grupo do antigo mascs (exceto o primeiro, 0)
		for i = 1:grau
			% Cada máscara do grupo i recebe bit do grupo (soma+i-1)
			pos = (i*n+1):((i+1)*n);
			mascs(pos) = bitxor(mascs(pos), bitshift(1, soma+i-1));
		end
		
		soma += grau;
	end
	
end
