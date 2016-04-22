/*
 * Copyright (c) 2016 Balabit
 * Copyright (c) 2016 Balázs Scheidler
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As an additional exemption you are allowed to compile & link against the
 * OpenSSL libraries as published by the OpenSSL project. See the file
 * COPYING for details.
 *
 */

#include "str-repr/list-scanner.h"
#include "str-repr/encode.h"

static void
_append_comma(GString *result)
{
  if (result->len == 0)
    return;

  if (result->str[result->len - 1] != ',')
    g_string_append_c(result, ',');
}

static void
tf_list_concat(LogMessage *msg, gint argc, GString *argv[], GString *result)
{
  ListScanner scanner;

  list_scanner_init(&scanner);
  list_scanner_input_gstring_array(&scanner, argc, argv);
  while (list_scanner_scan_next(&scanner))
    {
      _append_comma(result);
      str_repr_encode_append(result, list_scanner_get_current_value(&scanner), -1, ",");
    }
  list_scanner_deinit(&scanner);
}

TEMPLATE_FUNCTION_SIMPLE(tf_list_concat);

static void
tf_list_append(LogMessage *msg, gint argc, GString *argv[], GString *result)
{
  if (argc == 0)
    return;
  g_string_append_len(result, argv[0]->str, argv[0]->len);
  for (gint i = 1; i < argc; i++)
    {
      _append_comma(result);
      str_repr_encode_append(result, argv[i]->str, argv[i]->len, ",");
    }
}

TEMPLATE_FUNCTION_SIMPLE(tf_list_append);

static void
tf_list_head(LogMessage *msg, gint argc, GString *argv[], GString *result)
{
  ListScanner scanner;

  if (argc == 0)
    return;

  list_scanner_init(&scanner);
  list_scanner_input_gstring_array(&scanner, argc, argv);
  list_scanner_scan_next(&scanner);
  g_string_append(result, list_scanner_get_current_value(&scanner));
  list_scanner_deinit(&scanner);
}

TEMPLATE_FUNCTION_SIMPLE(tf_list_head);

static void
tf_list_tail(LogMessage *msg, gint argc, GString *argv[], GString *result)
{
  ListScanner scanner;

  if (argc == 0)
    return;

  list_scanner_init(&scanner);
  list_scanner_input_gstring_array(&scanner, argc, argv);
  list_scanner_scan_next(&scanner);

  while (list_scanner_scan_next(&scanner))
    {
      _append_comma(result);
      str_repr_encode_append(result, list_scanner_get_current_value(&scanner), -1, ",");
    }

  list_scanner_deinit(&scanner);
}

TEMPLATE_FUNCTION_SIMPLE(tf_list_tail);

static void
tf_list_count(LogMessage *msg, gint argc, GString *argv[], GString *result)
{
  ListScanner scanner;
  gint count = 0;

  if (argc != 0)
    {
      list_scanner_init(&scanner);
      list_scanner_input_gstring_array(&scanner, argc, argv);

      while (list_scanner_scan_next(&scanner))
        count++;

      list_scanner_deinit(&scanner);
    }

  format_uint32_padded(result, -1, ' ', 10, count);
}

TEMPLATE_FUNCTION_SIMPLE(tf_list_count);
