;yeh function hai markazi
kaam markazi@khali () karo
	rakho num@adad;
	rakho n@adad;
	rakho x@adad;
	rakho y@adad;
	rakho result@adad;
	lo << `Enter the number` >> x; input
	rakho r@adad := 7;
	rakho result := r - 2;
	dekhao << result;
	agar (x < 0) to phir karo
		dekhao << `munfi number daal diya\n`;
		wapas bhaijo 0;
	warna agar (x = 0) to phir
		dekhao << `yar 0 hai!\n`;
		wapas bhaijo 0;
	warna phir
		dekhao << `\nfibonacci series: `;
		rakho i@adad := 1;
		jab tak (i-1 >= n) karo
			dekhao << x << ` `;
			rakho z := x + y;
		bas karo
	bas karo
	wapas bhaijo 0;halt
kaam khatam