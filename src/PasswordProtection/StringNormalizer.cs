using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    public static class StringNormalizer
    {
        private static readonly char[] CharsToTrim = new[]
        {
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
            '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '{', '}', '[', ']', ';', ':',
            '\"', '\'', '<', '>', ',', '.', '?', '\\', '/', '+', '_', '=', '-','~','`'
        };

        public static string Normalize(string password)
        {
            password = password.ToLowerInvariant();
            password = new string(password.Where(t => !char.IsWhiteSpace(t)).ToArray());
            password = password.TrimEnd(StringNormalizer.CharsToTrim);
            password = password.TrimStart(StringNormalizer.CharsToTrim);
            return password
                .Replace("_", "")
                .Replace(".", "")
                .Replace("+", "")
                .Replace('$', 's')
                .Replace('0', 'o')
                .Replace('4', 'a')
                .Replace('3', 'e')
                .Replace('@', 'a')
                .Replace('4', 'a')
                .Replace('^', 'a')
                .Replace('(', 'c')
                .Replace('6', 'g')
                .Replace('1', 'i')
                .Replace('7', 't')
                .Replace('8', 'b')
                .Replace('2', 'z')
                .Replace('!', 'i');
        }
    }
}
